#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include "mat4.hpp"
#include "mat.hpp"
#include "test.hpp"
#include "vec4.hpp"
#include "glprog.hpp"
#include "Cameram.hpp"
#include "lines.hpp"
#include <ctime>
#include <vector>
#include "light.h"


/*
mat translate(mat, vec) m = tmat*mat, tmat =
 1 0 0 v1
 0 1 0 v2
 0 0 1 v3
 0 0 0  1

mat rotate(mat, vec axis, angle)
mat scale(mat, vec)
then mult rmat*vetrex

mat ortho(left, right, top, bottom, near, far) //cuts
mat frustum(left, right, top, bottom, near, far) //cut pyramide
mat perspective(field_of_view_y(angle), aspect_relative(=w/h), near, far)
r2mat

shader <- rmat*r2mat

struct Camera
float fovy, near, far
int w, h
vec position, targetview, side

  /
  | sidex, sidey, sidez, x
  | ypx, upy, upz, y
  | targetx, ty, tz, z
  | 0 0 0 1 ------ view
  \
 *
 x
 y
 z
 1

cameraMove(camera, vec)
cameraRotate(camera, vec, angle)
cameraScale(camera, vec)
cameraZoom(camera, float)
lookAt(vec pos, target, side)

 uniform mat4 mvp
 get uniform allocation

 glutkeyboardfunc
*/

/*
 * specExp(yarkost' pyatna obyazatel'no vstroit' v material)
 *
 *directional
 *  dir
 *  color
 *
 *point
 *  pos
 *  color
 *  attenuation(a, b, c) (1 / a+bd+cd^2)
 *
 *spot
 *  pos
 *  color
 *  dir
 *  zatuhanie
 *  angle
 *
 *ambient
 *
*/
/*
 * glGenBuffer
 * glBufferData(GL_ELEMENT_ARRAY_BUFFER)
 * glBindBuffer()
 * glDrawElements(GL_TRIANGLES, size, tosi-bosi) //instead of glDrawArrays
 *
 * TODO landscape
 * Perlin's noise (nooooouu)
 */

#define CAPTION ("Shch")

using namespace std;

const float WT = 1366./2;
const float HT = 768./2;
const float visibility = 1000.;
float speed = 1.;
float rotateSpeed = 1.;
const int h_lines = 26, v_lines = 26;

char gridFragmentShaderName[] = "../shaders/gridFragmentShader.glsl";
char gridVertexShaderName[] = "../shaders/gridVertexShader.glsl";
char infoFragmentShaderName[] = "../shaders/infoFragmentShader.glsl";
char infoVertexShaderName[] = "../shaders/infoVertexShader.glsl";
char cubeFragmentShaderName[] = "../shaders/cubeFragmentShader.glsl";

Mat4 MVP;
Mat4 model = Mat4::ident();
Mat4 view;
Mat4 projection;

vector<Camera*> cameras;
vector<Camera*>::iterator currentCam;
vector<Light>::iterator currentLight;

Camera *currentCamera;
Program gridProgram;
Program infoProgram;
Program cubeProgram;
Lines grid = Lines(h_lines + v_lines);
Cube cube = Cube(24., Vec4(4, 170., 24., 170.)), bigCube = Cube(480., Vec4(4, 0., -240)), bigBigCube(9600., Vec4(4, 0., -5300));
Buffer gridbuf, cubebuf, infobuf;
vector<Light> lights;
Camera Cam1 = Camera(Vec4(4, 25., 10., 25.), Vec4(4, 0., 10., 0.), Vec4(4, 0., 1., 0.));

namespace cg {
	const float areaR = 200.;
	void CreateGrid() {

		for(int i = 0; i < h_lines; i += 2) {
			grid.SetLine(    i, -areaR,  areaR/h_lines*i, areaR,  areaR/h_lines*i);
			grid.SetLine((i+1), -areaR, -areaR/h_lines*i, areaR, -areaR/h_lines*i);
		}
		for(int i = h_lines; i < h_lines + v_lines; i += 2) {
			grid.SetLine(    i,  areaR/v_lines*(i - h_lines), areaR,  areaR/v_lines*(i - h_lines), -areaR);
			grid.SetLine((i+1), -areaR/v_lines*(i - h_lines), areaR, -areaR/v_lines*(i - h_lines), -areaR);
		}
	}
}

namespace lts {
	void SetCamera() {
		currentCam = cameras.begin() + (lights.size() > 0 ? std::distance(lights.begin(), currentLight) + 1 : 0);
		currentCamera = *currentCam;
	}
	void DeleteCurrentLight() {
		if (lights.size() < 1) return;
		currentLight = lights.erase(currentLight);
	}
	void IncreaseBrightness() {
		if (lights.size() < 1) return;
		(*currentLight).intense += 0.2;
	}
	void DecreaseBrightness() {
		if (lights.size() < 1) return;
		(*currentLight).intense = fabs((*currentLight).intense - (*currentLight).intense > 0 ? 0.2 : 0);
	}
}

struct CamInfo {
	Lines lines = Lines(7);
	const int ox = 0, oy = 1, oz = 2, dir = 3, pitch = 4, ht = 5, htscale = 6;
	void Draw(Camera cam, Program prog) {
		float a = cam.direction[0] + 0.5, b = cam.direction[2], x = sqrt(a*a + b*b);
		lines.SetLine(dir, 0.5, .0, 0., a, -b, 0.);
		lines.SetLine(pitch,  0., 0., 0., 0.5*cam.up.dot(cam.worldUp()), -cam.direction[1], 0.);
		lines.SetLine(ht, -0.7, cam.position[1]/100, 0., -0.3, cam.position[1]/100, 0.);
		lines.FillBuffer(&infobuf, infoProgram);
		infobuf.Draw(infoProgram);
	}
	void Init() {
		lines.SetLine(ox, .3,  0.,  0., .7, 0., 0.);
		lines.SetLine(oy, .5 , -.7,  0., .5, .7, 0.);
		lines.SetLine(oz, 0. , -.7,  0., 0., .7, 0.);
		lines.SetLine(htscale, -.5, -.7, 0., -.5, .7, 0.);
	}
} info;

bool isPressed[256];
inline void handleKeys() {
	if (isPressed['w']) {
		currentCamera->MoveForward(-speed);
	}
	if (isPressed[(int)'q']) {
		currentCamera->Rotate(currentCamera->worldUp(), rotateSpeed);
	}
	if (isPressed[(int)'s']) {
		currentCamera->MoveForward(speed);
	}
	if (isPressed[(int)'e']) {
		currentCamera->Rotate(currentCamera->worldUp(), -rotateSpeed);
	}
	if (isPressed[(int)'z']) {
		currentCamera->MoveUp(speed);
	}
	if (isPressed[(int)'x']) {
		currentCamera->MoveUp(-speed);
	}
	if (isPressed[(int)'a']) {
		currentCamera->MoveSideway(-speed);
	}
	if (isPressed[(int)'d']) {
		currentCamera->MoveSideway(speed);
	}
	if (false && isPressed[(int)'`']) {
		currentCamera->Rotate(currentCamera->rightHand(), rotateSpeed);
	}
	if (false && isPressed[(int)'`']) {
		currentCamera->Rotate(currentCamera->rightHand(), -rotateSpeed);
	}
	if (isPressed[(int)'r']) {
		speed = (abs((int)(speed*100) + 2)%1000)/100.;
	}
	if (isPressed[(int)'f']) {
		speed = (abs((int)(speed*100) - 2)%1000)/100.;
	}
	if (isPressed[(int)'t']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) + 1)%1000)/100.;
	}
	if (isPressed[(int)'g']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) - 1)%1000)/100.;
	}
	if (isPressed[(int)'c']) {
		projection = currentCamera->projectionMatrix(currentCamera->zoom += 0.4, WT/HT, 0.1, visibility);
	}
	if (isPressed[(int)'v']) {
		projection = currentCamera->projectionMatrix(currentCamera->zoom -= 0.4, WT/HT, 0.1, visibility);
	}
}

inline void Render() {
	handleKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gridProgram.Use();
	view = currentCamera->GetView();
	cout << *currentCamera << "S: " << setprecision(3) << speed << "mph " << rotateSpeed << "rad\n\n" << endl;
	MVP  = projection * view * model;
	gridProgram.UniformMatrix(MVP.transposed().data, gridProgram.Location("trans", 1));
	gridbuf.Draw(gridProgram);

	cubeProgram.Use();
	cubeProgram.UniformInt(lights.size(), cubeProgram.Location("lightamt", 1));
	cubeProgram.UniformVec(currentCamera->position, cubeProgram.Location("eye", 1));
	for(int i = 0; i < lights.size(); i++)
		lights.at(i).Uniform(i, cubeProgram);
	cubeProgram.UniformMatrix(MVP.transposed().data, cubeProgram.Location("trans", 1));
	cubeProgram.UniformMatrix((model).data, cubeProgram.Location("model", 1));
	cube.FillBuffer(&cubebuf, cubeProgram);
	cubebuf.Draw(cubeProgram);
	bigCube.FillBuffer(&cubebuf, cubeProgram);
	cubebuf.Draw(cubeProgram);

	infoProgram.Use();
	MVP = Mat4::ident().translated(Vec4(4, .7, .7)).scale(Vec4(4, .3, .3, .3));
	infoProgram.UniformMatrix(MVP.transposed().data, infoProgram.Location("trans", 1));
	info.Draw(*currentCamera, infoProgram);

	glutSwapBuffers();
}

void handleKey(unsigned char key, int x, int y) {
	isPressed[(int)key] = true;
	if (key == '.') {
		if (lights.size() > 0 && ++currentLight == lights.end())
			currentLight = lights.begin();
//		projection = currentCamera->projectionMatrix(currentCamera->zoom, WT/HT, 0.1, visibility);
	}
	if (key == ',') {
		if (lights.size() > 0 && currentLight-- == lights.begin())
			currentLight = lights.end() - 1;
//		projection = currentCamera->projectionMatrix(currentCamera->zoom, WT/HT, 0.1, visibility);
	}
	if (key == 'l') {

	}
	using namespace lts;
	if (key == '0') {
		DeleteCurrentLight();
	}
	if (key == '=') {
		IncreaseBrightness();
	}
	if (key == '-') {
		DecreaseBrightness();
	}
	if (key == '0' + DIR) {
		lights.push_back(Light(currentCamera->direction, Vec4(4, 1., 1. ,1.), 1.));
		currentLight = lights.end() - 1;
	}
	if (key == '0' + POINT) {
		lights.push_back(Light(*currentCamera, currentCamera->direction, 20.));
		currentLight = lights.end() - 1;
	}
	if (key == '0' + SPOT) {
		lights.push_back(Light(*currentCamera, currentCamera->direction, 20., 10.));
		currentLight = lights.end() - 1;
	}

}

void handleUpKey(unsigned char key, int x, int y) {
	isPressed[(int)key] = false;
}

void handleSpecKey(int key, int x, int y) {
}

void handleUpSpecKey(int key, int x, int y) {
}

inline void handleMouse(int x, int y) {
	static int cx = WT/2, cy = HT/2;
	if (x != cx || y != cy) {
		int dx = cx - x, dy = cy - y;
		currentCamera->Rotate(currentCamera->worldUp(), dx*rotateSpeed);
		currentCamera->Rotate(currentCamera->rightHand(), dy*rotateSpeed);
		glutWarpPointer(cx, cy);
	}
}

void timer(int value) {
	handleKeys();
	glutPostRedisplay();
	glutTimerFunc(30, timer, 0);
}

void mouseWheel(int button, int dir, int x, int y) {
	projection = currentCamera->projectionMatrix(currentCamera->zoom += dir*0.5, WT/HT, 0.1, visibility);
	(cout << "!!!").flush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(WT, HT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(CAPTION);
	glutDisplayFunc(Render);
	glutKeyboardFunc(handleKey);
	glutKeyboardUpFunc(handleUpKey);
	glutSpecialFunc(handleSpecKey);
	glutSpecialUpFunc(handleUpSpecKey);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutPassiveMotionFunc(handleMouse);
	glutSetCursor(GLUT_CURSOR_NONE);
	glEnable(GL_DEPTH_TEST);
	glutMouseWheelFunc(mouseWheel);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		return 1;

	gridProgram = Program(
		Shader(gridVertexShaderName, GL_VERTEX_SHADER),
		Shader(gridFragmentShaderName, GL_FRAGMENT_SHADER));

	infoProgram = Program(
		Shader(infoVertexShaderName, GL_VERTEX_SHADER),
		Shader(infoFragmentShaderName, GL_FRAGMENT_SHADER));

	cubeProgram = Program(
		Shader(gridVertexShaderName, GL_VERTEX_SHADER),
		Shader(cubeFragmentShaderName, GL_FRAGMENT_SHADER));

	cg::CreateGrid();

	cameras.push_back(&Cam1);
	currentCam = cameras.begin();
	currentCamera = *currentCam;

	gridbuf.Init();
	cubebuf.Init();
	infobuf.Init();
	float backg = 0.6;
	glClearColor(backg*.136, backg*.204, backg*.252, 0.5f);
	projection = currentCamera->projectionMatrix(45., WT/HT, 0.1, visibility);

	info.Init();

	grid.FillBuffer(&gridbuf, gridProgram);
	cube.FillBuffer(&cubebuf, cubeProgram);

	glutTimerFunc(30, timer, 0);
	glutMainLoop();
	return 0;
}
