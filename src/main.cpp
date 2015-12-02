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
#include <SOIL/SOIL.h>

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
char textureName[] = "../textures/water1.jpg";
char lightSourceFragmentShader[] = "../shaders/lightSourceFragmentShader.glsl";

char lightType[][20] = {"", "DIR", "SPOT", "POINT"};

Mat4 MVP;
Mat4 model = Mat4::ident(), lmodel;
Mat4 view;
Mat4 projection;

GLuint texture;

vector<Camera*> cameras;
vector<Light>::iterator currentLight;

Camera *currentCamera;
Camera *currentViewCamera;
Program gridProgram;
Program infoProgram;
Program cubeProgram;
Program lsourceProgram;
Lines grid = Lines(h_lines + v_lines);
Cube cube = Cube(24., Vec4(4, 170., 24., 170.)), bigCube = Cube(480., Vec4(4, 0., -240)), bigBigCube(9600., Vec4(4, 0., -5300));
Buffer gridbuf, cubebuf, infobuf, lcubebuf;
vector<Light> lights;
Camera Cam1 = Camera(Vec4(4, 25., 10., 25.), Vec4(4, 0., 10., 0.), Vec4(4, 0., 1., 0.));

Plane plane(Vec4(4, 0., 200., 0.), 200.);

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
	bool isLightView = false;
	bool isLightControl = false;
	void SetCamera() {
		if (lights.size() < 1) return;
		currentCamera = &((*currentLight).state);
	}
	void DeleteCurrentLight() {
		if (lights.size() < 1) return;
		currentLight = lights.erase(currentLight);
		if (currentLight == lights.end())
			currentLight = lights.begin();
	}
	void IncreaseBrightness() {
		if (lights.size() < 1) return;
		(*currentLight).intense += 0.2;
	}
	void DecreaseBrightness() {
		if (lights.size() < 1) return;
		(*currentLight).intense -= (*currentLight).intense - 0.2 > 0 ? 0.2 : 0;
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
	if (isPressed[(int)'y']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) - 1)%1000)/100.;
	}
	if (isPressed[(int)'c']) {
		projection = currentCamera->projectionMatrix(currentCamera->zoom += 0.4, WT/HT, 0.1, visibility);
	}
	if (isPressed[(int)'v']) {
		projection = currentCamera->projectionMatrix(currentCamera->zoom -= 0.4, WT/HT, 0.1, visibility);
	}
	if (isPressed[(int)'=']) {
		lts::IncreaseBrightness();
	}
	if (isPressed[(int)'-']) {
		lts::DecreaseBrightness();
	}
}

inline void Render() {
	handleKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gridProgram.Use();
	view = currentViewCamera->GetView();
	MVP  = projection * view * model;
	gridProgram.UniformMatrix(MVP.transposed().data, gridProgram.Location("trans", 1));
	gridbuf.Draw(gridProgram);

	cubeProgram.Use();
	cubeProgram.UniformInt(lights.size(), cubeProgram.Location("lightamt", 1));
	cubeProgram.UniformVec(currentViewCamera->position, cubeProgram.Location("eye", 1));
	for(int i = 0; i < lights.size(); i++)
		lights.at(i).Uniform(i, cubeProgram);
	cubeProgram.UniformMatrix(MVP.transposed().data, cubeProgram.Location("trans", 1));
	cubeProgram.UniformMatrix((model).data, cubeProgram.Location("model", 1));

	cube.FillBuffer(&cubebuf, cubeProgram);
	cubebuf.DrawElements(cubeProgram, 36, cube.indexes, cube.vertex);
	bigCube.FillBuffer(&cubebuf, cubeProgram);
	cubebuf.DrawElements(cubeProgram, 36, bigCube.indexes, bigCube.vertex);

	plane.FillBuffer(&cubebuf, cubeProgram);
	cubebuf.DrawElements(cubeProgram, 6, plane.indexes, plane.vertex_normals);

	lsourceProgram.Use();
	for(int i = 0; i < lights.size(); i++) {
		if (lights[i].type == DIR || currentViewCamera == &lights[i].state) continue;
		lmodel = Mat4::ident().translated(lights[i].state.position);
		lsourceProgram.UniformVec(lights[i].color, lsourceProgram.Location("incolor", 1));
		lsourceProgram.UniformFloat(lights[i].intense, lsourceProgram.Location("intense", 1));
		lsourceProgram.UniformMatrix((projection*view*lmodel).transposed().data, lsourceProgram.Location("trans", 1));
		lights[i].source.FillBuffer(&lcubebuf, lsourceProgram);
		lights[i].source.FillIndexBuffer(&lcubebuf, lsourceProgram);
		lcubebuf.DrawElements(lsourceProgram, 36, lights[i].source.indexes, lights[i].source.vertex);
	};

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
	}
	if (key == ',') {
		if (lights.size() > 0 && currentLight-- == lights.begin())
			currentLight = lights.end() - 1;
	}
	using namespace lts;
	if (key == 'l') {
		SetCamera();
		isLightView = true;
		isLightControl = true;
		currentViewCamera = currentCamera;
	}
	if (key == 'k') {
		currentViewCamera = currentCamera = &Cam1;
		isLightView = false;
		isLightControl = false;
	}
	if (key == 'o') {
		if (lights.size() > 0) {
			isLightControl = !isLightControl;
		}
		if (isLightControl)
			SetCamera();
		else
			currentCamera = &Cam1;
	}
	if (key == '0') {
		DeleteCurrentLight();
	}
	if (key == '=') {
	}
	if (key == '-') {
	}
	if (key == 'g') {
		(*currentLight).color += Vec4(4, 0.05);
		(*currentLight).color.normalize();
	}
	if (key == 'h') {
		(*currentLight).color += Vec4(4, 0, 0.05);
		(*currentLight).color.normalize();
	}
	if (key == 'j') {
		(*currentLight).color += Vec4(4, 0, 0, 0.05);
		(*currentLight).color.normalize();
	}
	if (key == 'b') {
		(*currentLight).color -= Vec4(4, 0.05);
		(*currentLight).color.normalize();
	}
	if (key == 'n') {
		(*currentLight).color -= Vec4(4, 0, 0.05);
		(*currentLight).color.normalize();
	}
	if (key == 'm') {
		(*currentLight).color -= Vec4(4, 0, 0, 0.05);
		(*currentLight).color.normalize();
	}
	if (key == '0' + DIR) {
		lights.push_back(Light(*currentCamera, currentCamera->direction, 1.05, DIR));
		currentLight = lights.end() - 1;
	}
	if (key == '0' + POINT) {
		lights.push_back(Light(*currentCamera, currentCamera->direction, 20., POINT));
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
	static Light clight;
	handleKeys();
	//cout << *currentCamera << "S: " << setprecision(3) << speed << "mph " << rotateSpeed << "rad" << endl;
	if (lights.size() > 0) {
		clight = *currentLight;
		cout << "\ncurrent light: " << std::distance(lights.begin(), currentLight) << "_" << lightType[clight.type] << endl;
		cout << "brightness = " << clight.intense << "\ncolor = " << clight.color.transposed() << "\n\n";
	}
	glutPostRedisplay();
	glutTimerFunc(40, timer, 0);
}

void mouseWheel(int button, int dir, int x, int y) {
	projection = currentCamera->projectionMatrix(currentCamera->zoom += dir*0.5, WT/HT, 0.1, visibility);
	(cout << "!!!").flush();
}

void LoadTextures() {
	int texh, texw;
	unsigned char* image =
	    SOIL_load_image(textureName, &texw, &texh, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texw, texh, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenTextures(1, &texture);

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

	lsourceProgram = Program(
		Shader(infoVertexShaderName, GL_VERTEX_SHADER),
		Shader(lightSourceFragmentShader, GL_FRAGMENT_SHADER));

	cg::CreateGrid();

	cameras.push_back(&Cam1);
	currentViewCamera = currentCamera = &Cam1;

	gridbuf.Init();
	cubebuf.Init();
	infobuf.Init();
	lcubebuf.Init();

	float backg = 0.6;
	glClearColor(backg*.136, backg*.204, backg*.252, 0.5f);
	projection = currentCamera->projectionMatrix(45., WT/HT, 0.1, visibility);

	info.Init();

	plane.FillIndexBuffer(&cubebuf, cubeProgram);
	plane.FillBuffer(&cubebuf, cubeProgram);

	cube.FillIndexBuffer(&cubebuf, cubeProgram);
	grid.FillBuffer(&gridbuf, gridProgram);
	cube.FillBuffer(&cubebuf, cubeProgram);

	glutTimerFunc(40, timer, 0);
	glutMainLoop();
	return 0;
}
