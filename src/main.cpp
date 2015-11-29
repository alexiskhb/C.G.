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
const int h_lines = 100, v_lines = 100;

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

Camera *currentCamera;
Program gridProgram;
Program infoProgram;
Program cubeProgram;
Lines grid = Lines(h_lines + v_lines);
Cube cube = Cube(24., Vec4(4, 170., 24., 170.));
Buffer gridbuf, cubebuf, infobuf;
vector<Light> lights;

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

struct CamInfo {
	Lines lines = Lines(7);
	const int ox = 0, oy = 1, oz = 2, dir = 3, pitch = 4, ht = 5, htscale = 6;
	void Draw(Camera cam, Program prog) {
		float a = cam.forward[0] + 0.5, b = cam.forward[2], x = sqrt(a*a + b*b);
		lines.SetLine(dir, 0.5, .0, 0., a, -b, 0.);
		lines.SetLine(pitch,  0., 0., 0., 0.5*cam.up.dot(cam.worldUp()), -cam.forward[1], 0.);
		lines.SetLine(ht, -0.7, cam.position[1]/100, 0., -0.3, cam.position[1]/100, 0.);
		lines.FillBuffer(&infobuf, infoProgram);
		infobuf.Draw(infoProgram);
	}
} info;

bool isPressed[256];
inline void handleKeys() {
	if (isPressed['w']) {
		currentCamera->MoveForward(-speed);
	}
	if (isPressed[(int)'a']) {
		currentCamera->Rotate(currentCamera->worldUp(), rotateSpeed);
	}
	if (isPressed[(int)'s']) {
		currentCamera->MoveForward(speed);
	}
	if (isPressed[(int)'d']) {
		currentCamera->Rotate(currentCamera->worldUp(), -rotateSpeed);
	}
	if (isPressed[(int)'z']) {
		currentCamera->MoveUp(speed);
	}
	if (isPressed[(int)'x']) {
		currentCamera->MoveUp(-speed);
	}
	if (isPressed[(int)'q']) {
		currentCamera->MoveSideway(-speed);
	}
	if (isPressed[(int)'e']) {
		currentCamera->MoveSideway(speed);
	}
	if (isPressed[(int)'u']) {
		currentCamera->Rotate(currentCamera->rightHand(), rotateSpeed);
	}
	if (isPressed[(int)'j']) {
		currentCamera->Rotate(currentCamera->rightHand(), -rotateSpeed);
	}
	if (isPressed[(int)'p']) {
		speed = (abs((int)(speed*100) + 15)%1000)/100.;
	}
	if (isPressed[(int)'o']) {
		speed = (abs((int)(speed*100) - 15)%1000)/100.;
	}
	if (isPressed[(int)'l']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) + 5)%1000)/100.;
	}
	if (isPressed[(int)'k']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) - 5)%1000)/100.;
	}
	if (isPressed[(int)'n']) {
		projection = currentCamera->projectionMatrix(currentCamera->zoom += 0.4, WT/HT, 0.1, visibility);
	}
	if (isPressed[(int)'b']) {
		projection = currentCamera->projectionMatrix(currentCamera->zoom -= 0.4, WT/HT, 0.1, visibility);
	}
}

inline void Render() {
	handleKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gridProgram.Use();
	view = currentCamera->GetView();
	cout << *currentCamera << view << endl << "S: " << setprecision(3) << speed << "mph " << rotateSpeed << "rad\n\n\n\n";
	MVP  = projection * view * model;
	gridProgram.UniformMatrix(MVP.transposed().data, gridProgram.Location("trans", 1));
	gridbuf.Draw(gridProgram);

	cubeProgram.Use();

	cubeProgram.UniformMatrix(MVP.transposed().data, cubeProgram.Location("trans", 1));
	cubeProgram.UniformMatrix(model.transposed().data, gridProgram.Location("model", 1));
	cubebuf.Draw(cubeProgram);

	infoProgram.Use();
	MVP = Mat4::ident().translated(Vec4(4, .7, .7)).scale(Vec4(4, .3, .3, .3));
	infoProgram.UniformMatrix(MVP.transposed().data, infoProgram.Location("trans", 1));
	info.Draw(*currentCamera, infoProgram);

	glutSwapBuffers();
}

void handleKey(unsigned char key, int x, int y) {
	isPressed[(int)key] = true;
	if (key == 'm') {
		if (++currentCam == cameras.end())
			currentCam = cameras.begin();
		currentCamera = *currentCam;
		projection = currentCamera->projectionMatrix(currentCamera->zoom, WT/HT, 0.1, visibility);
	}
}

void handleUpKey(unsigned char key, int x, int y) {
	isPressed[(int)key] = false;
}

void handleSpecKey(int key, int x, int y) {
}

void handleUpSpecKey(int key, int x, int y) {
}

int lastTime = 0;
int warping = 0;

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
	GLenum res = glewInit();
	if (res != GLEW_OK)
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

	Camera
			Cam1 = Camera(Vec4(4, 25., 10., 25.), Vec4(4, 0., 10., 0.), Vec4(4, 0., 1., 0.)),
			Cam2 = Camera(Vec4(4, 0., 10., -25.), Vec4(4, 0., 10., 0.), Vec4(4, 0., 1., 0.));

	Vec4 vector = Vec4(4, 1.);

	cameras.push_back(&Cam1);
	cameras.push_back(&Cam2);

	currentCam = cameras.begin();
	currentCamera = *currentCam;

	gridbuf.Init();
	cubebuf.Init();
	infobuf.Init();
//	glClearColor(0.24f, 0.05f, 0.18f, 0.5f);
	float backg = 1.1;
	glClearColor(backg*.136, backg*.204, backg*.252, 0.5f);
//	glClearColor(backg*.244, backg*.122, backg*.0, 0.5f);
	projection = currentCamera->projectionMatrix(45., WT/HT, 0.1, visibility);

	info.lines.SetLine(info.ox, .3,  0.,  0., .7, 0., 0.);
	info.lines.SetLine(info.oy, .5 , -.7,  0., .5, .7, 0.);
	info.lines.SetLine(info.oz, 0. , -.7,  0., 0., .7, 0.);
	info.lines.SetLine(info.htscale, -.5, -.7, 0., -.5, .7, 0.);

	grid.FillBuffer(&gridbuf, gridProgram);
	cube.FillBuffer(&cubebuf, cubeProgram);

	glutTimerFunc(30, timer, 0);
	glutMainLoop();


	return 0;
}
