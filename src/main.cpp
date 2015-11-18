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

#define CAPTION ("Shch")

using namespace std;

const float WT = 1366./2;
const float HT = 768./2;
float speed = 0.55;
float rotateSpeed = 1.15;
const int h_lines = 100, v_lines = 100;

char gridFragmentShaderName[] = "../shaders/gridFragmentShader.hlsl";
char gridVertexShaderName[] = "../shaders/gridVertexShader.hlsl";
char infoFragmentShaderName[] = "../shaders/infoFragmentShader.hlsl";
char infoVertexShaderName[] = "../shaders/infoVertexShader.hlsl";

Mat4 MVP;
Mat4 model = Mat4::ident();
Mat4 view;
Mat4 projection;

Camera camera;
Program gridProgram;
Program infoProgram;
Lines grid = Lines(h_lines + v_lines);

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
	Lines lines = Lines(7*2*3);
	const int ox = 0, oy = 1, oz = 2, dir = 3, pitch = 4, ht = 5, htscale = 6;
	void Draw(Camera cam, Program prog) {
		lines.SetLine(dir, 0.5, .0, 0., cam.forward[0] + 0.5, -cam.forward[2], 0.);
		lines.SetLine(pitch,  0., 0., 0., 0.5*cam.up.dot(cam.worldUp()), -cam.forward[1], 0.);
		lines.SetLine(ht, -0.7, cam.position[1]/100, 0., -0.3, cam.position[1]/100, 0.);
		lines.Draw(prog);
	}
} info;

bool isPressed[256];
void handleKeys() {
	if (isPressed[(int)'w']) {
		camera.MoveForward(-speed);
	}
	if (isPressed[(int)'a']) {
		camera.Rotate(camera.worldUp(), rotateSpeed);
	}
	if (isPressed[(int)'s']) {
		camera.MoveForward(speed);
	}
	if (isPressed[(int)'d']) {
		camera.Rotate(camera.worldUp(), -rotateSpeed);
	}
	if (isPressed[(int)'z']) {
		camera.MoveUp(speed);
	}
	if (isPressed[(int)'x']) {
		camera.MoveUp(-speed);
	}
	if (isPressed[(int)'q']) {
		camera.MoveSideway(-speed);
	}
	if (isPressed[(int)'e']) {
		camera.MoveSideway(speed);
	}
	if (isPressed[(int)'u']) {
		camera.Rotate(camera.rightHand(), rotateSpeed);
	}
	if (isPressed[(int)'j']) {
		camera.Rotate(camera.rightHand(), -rotateSpeed);
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
}

void Render() {
	handleKeys();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gridProgram.Use();
	gridProgram.FillUniform4fv("trans", MVP.data);
	gridProgram.GetAttribAllocation("vertexPosition");
	view = camera.GetView();
	cout << camera << view << endl << "S: " << setprecision(3) << speed << "mph " << rotateSpeed << "rad\n\n\n\n";
	MVP  = projection * view * model;
	gridProgram.UniformMatrix(MVP.transposed().data);
	grid.Draw(gridProgram);


	infoProgram.Use();
	infoProgram.GetAttribAllocation("vertexPosition");
	MVP = Mat4::ident().translated(Vec4(4, .7, .7)).scale(Vec4(4, .3, .3, .3));
	infoProgram.FillUniform4fv("trans", MVP.data);
	infoProgram.UniformMatrix(MVP.transposed().data);
	info.Draw(camera, infoProgram);

	glutSwapBuffers();
}

void handleKey(unsigned char key, int x, int y) {
	isPressed[(int)key] = true;
	Render();
}

void handleUpKey(unsigned char key, int x, int y) {
	isPressed[(int)key] = false;
	Render();
}

void handleMouse(int x, int y) {
	static int prevx, prevy;
	//(cout << x << ' ' << y << endl).flush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(WT, HT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(CAPTION);
	glutDisplayFunc(Render);
	glutKeyboardFunc(handleKey);
	glutKeyboardUpFunc(handleUpKey);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	glutPassiveMotionFunc(handleMouse);
	glEnable(GL_DEPTH_TEST);
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


	cg::CreateGrid();
	camera = Camera(Vec4(4, 0., 1., 0.), Vec4(4, 4., 1., 4.), Vec4(4, 0., 1., 0.));

	glGenVertexArrays(1, &grid.vertexArray);
	glGenBuffers(1, &grid.buffer);
	projection = camera.projectionMatrix(45., WT/HT, 0.1, 400.);

	glClearColor(0.24f, 0.05f, 0.18f, 0.5f);
//	glClearColor(3*.136, 3*.194, 3*.252, 0.5f);

	glGenVertexArrays(1, &info.lines.vertexArray);
	glGenBuffers(1, &info.lines.buffer);
	info.lines.SetLine(info.ox, -.0,  0.,  0., .0, 0., 0.);
	info.lines.SetLine(info.oy, .5 , -.9,  0., .5, .9, 0.);
	info.lines.SetLine(info.oz, 0. , -.9,  0., 0., .9, 0.);//there is no mistake.
	info.lines.SetLine(info.htscale, -.5, -.9, 0., -.5, .9, 0.);

	glutMainLoop();

	return 0;
}
