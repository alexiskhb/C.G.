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
float speed = 0.15;
float rotateSpeed = 1.15;

char fragmentShaderName[] = "../shaders/FragmentShader.hlsl";
char linesShaderName[] = "../shaders/LinesVertices.hlsl";

GLuint *vertexArrays;
GLuint *buffer;
Program gridProgram;

Mat4 MVP;
Mat4 model = Mat4::ident();
Mat4 view;
Mat4 projection;

Camera camera;

namespace cl {
	const int h_lines = 100, v_lines = 100;
	const float areaR = 200.;
	float lines[h_lines*2*3 + v_lines*2*3];

	void SetLine(int st, floatv x1, floatv y1, floatv x2, floatv y2) {
		int e = st + 3;
		lines[st    ] = x1;
		lines[st + 1] = 0.;
		lines[st + 2] = y1;
		lines[e     ] = x2;
		lines[e  + 1] = 0.;
		lines[e  + 2] = y2;
	}

	void SetLine(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2) {
		SetLine(st, x1, y1, x2, y2);
		int e = st + 3;
		lines[st + 2] = z1;
		lines[e  + 2] = z2;
	}

	void CreateLines() {
		SetLine(0,    .5, .5, .5,  .5, -.5, .5);
		SetLine(6,   .5, -.5, .5, -.5, -.5, .5);
		SetLine(12, -.5, -.5, .5, -.5,  .5, .5);
		SetLine(18,  -.5, .5, .5,  .5,  .5, .5);

		SetLine(24,   .5, .5, -.5,  .5, -.5, -.5);
		SetLine(30,  .5, -.5, -.5, -.5, -.5, -.5);
		SetLine(36, -.5, -.5, -.5, -.5,  .5, -.5);
		SetLine(42,  -.5, .5, -.5,  .5,  .5, -.5);

		SetLine(48, .5,  .5, .5, .5, .5,  -.5);
		SetLine(54, .5, -.5, .5, .5, -.5, -.5);

//		return;
		for(int i = 0; i < h_lines; i += 2) {
			SetLine(    i*6, -areaR,  areaR/h_lines*i, areaR,  areaR/h_lines*i);
			SetLine((i+1)*6, -areaR, -areaR/h_lines*i, areaR, -areaR/h_lines*i);
		}
		for(int i = h_lines; i < h_lines + v_lines; i += 2) {
			SetLine(    i*6,  areaR/v_lines*(i - h_lines), areaR,  areaR/v_lines*(i - h_lines), -areaR);
			SetLine((i+1)*6, -areaR/v_lines*(i - h_lines), areaR, -areaR/v_lines*(i - h_lines), -areaR);
		}
	}
}

struct CamInfo {
	int a = 5;
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
		speed = (abs((int)(speed*100) + 15)%300)/100.;
	}
	if (isPressed[(int)'o']) {
		speed = (abs((int)(speed*100) - 15)%300)/100.;
	}
	if (isPressed[(int)'l']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) + 5)%100)/100.;
	}
	if (isPressed[(int)'k']) {
		rotateSpeed = (abs((int)(rotateSpeed*100) - 5)%100)/100.;
	}
}

void Render() {
	handleKeys();
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gridProgram.Use();
	view = camera.GetView();
	cout << camera << view << endl << "S: " << setprecision(3) << speed << "mph " << rotateSpeed << "rad\n\n\n\n";
	MVP  = projection * view * model;
	gridProgram.UniformMatrix(MVP.transposed().data);
	glBindVertexArray(vertexArrays[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, (cl::h_lines*2*3 + cl::v_lines*2*3) * sizeof(float), cl::lines, GL_STATIC_DRAW);
	gridProgram.EnableVertexAttribArray();
	gridProgram.VertexAttribPointer(3, GL_FLOAT, GL_FALSE, 0, 0);
	for(int i = 0; i < cl::h_lines + cl::v_lines; i++)
		glDrawArrays(GL_LINES, i*2, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);
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
		Shader(linesShaderName, GL_VERTEX_SHADER),
		Shader(fragmentShaderName, GL_FRAGMENT_SHADER));

	cl::CreateLines();
	camera = Camera(
			/*position*/Vec4(4, 0., 0., 0.),
			/*target*/  Vec4(4, 4., 0., 4.),
			/*up*/      Vec4(4, 0., 1., 0.));

	projection = camera.projectionMatrix(45., WT/HT, 0.1, 400.);

	vertexArrays = new GLuint[1];
	glGenVertexArrays(1, vertexArrays);
	buffer = new GLuint[1];
	glGenBuffers(1, buffer);
	gridProgram.Use();
	gridProgram.GetAttribAllocation("vertexPosition");
	gridProgram.EnableVertexAttribArray();
	gridProgram.FillUniform4fv("trans", MVP.data);


	glClearColor(0.24f, 0.05f, 0.18f, 0.5f);
//	glClearColor(3*.136, 3*.194, 3*.252, 0.5f);

	glutMainLoop();

	return 0;
}
//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
//	Vec4 v = Vec4(2, 5.1, 6.6);
