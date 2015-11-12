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

const int WT = 1366./2;
const int HT = 768./2;

float stepY = 0.5;
float stepX = 0.5;
float angleZ = 0.;
float angleX = 0.;

char fragmentShaderName[] = "../shaders/FragmentShader.hlsl";
char linesShaderName[] = "../shaders/LinesVertices.hlsl";

GLuint *vertexArrays;
GLuint *buffer;
Program lsProgram;

Mat4 MVP;
Mat4 model;
Mat4 view;
Mat4 projection;

Camera camera;

const int h_lines = 500, v_lines = 500;
float lines[h_lines*2*3 + v_lines*2*3];

namespace cl {
	void SetLine(int st, floatv x1, floatv y1, floatv x2, floatv y2) {
		int e = st + 3;
		lines[st    ] = x1;
		lines[st + 1] = y1;
		lines[st + 2] = 0.;
		lines[e     ] = x2;
		lines[e  + 1] = y2;
		lines[e  + 2] = 0.;
	}

	void CreateLines() {
		for(int i = 0; i < h_lines; i += 2) {
			SetLine(    i*6, -100.,  100./h_lines*i, 100.,  100./h_lines*i);
			SetLine((i+1)*6, -100., -100./h_lines*i, 100., -100./h_lines*i);
		}
		for(int i = h_lines; i < h_lines + v_lines; i += 2) {
			SetLine(    i*6,  100./v_lines*(i - h_lines), 100.,  100./v_lines*(i - h_lines), -100.);
			SetLine((i+1)*6, -100./v_lines*(i - h_lines), 100., -100./v_lines*(i - h_lines), -100.);
		}
	}
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	lsProgram.Use();
	std::cout << camera;
	view = camera.GetView();
	MVP  = projection * view * model;
	lsProgram.UniformMatrix(MVP);
	glBindVertexArray(vertexArrays[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, (h_lines*2*3 + v_lines*2*3) * sizeof(float), lines, GL_STATIC_DRAW);
	lsProgram.EnableVertexAttribArray();
	lsProgram.VertexAttribPointer(3, GL_FLOAT, GL_FALSE, 0, 0);
	for(int i = 0; i < h_lines + v_lines; i++)
		glDrawArrays(GL_LINES, i*2, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();
}

void handleKey(unsigned char key, int x, int y) {
	if (key == 'w') {
		camera.MoveForward(0.05);
	}
	if (key == 'a') {
		camera.Rotate(Vec4(4, 0., 0., 1.), angleZ -= 1.);
	}
	if (key == 's') {
		camera.MoveForward(-0.05);
	}
	if (key == 'd') {
		camera.Rotate(Vec4(4, 0., 0., 1.), angleZ += 1.);
	}
	if (key == 'z') {
	}
	if (key == 'x') {
	}
	if (key == 'q') {
		camera.MoveSideway(-0.05);
	}
	if (key == 'e') {
		camera.MoveSideway(0.05);
	}
	if (key == 'u') {
		camera.Rotate(Vec4(4, 1., 0., 0.), angleX += 1.);
	}
	if (key == 'j') {
		camera.Rotate(Vec4(4, 1., 0., 0.), angleX -= 1.);
	}

	Render();
}


float mtr(int i, int j, int h, int w) {
	return fabs(i + j*2);
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
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
		return 1;

	lsProgram = Program(
		Shader(linesShaderName, GL_VERTEX_SHADER),
		Shader(fragmentShaderName, GL_FRAGMENT_SHADER));

//	perspective = Mat4::ident().perspective(Vec4(4, 0., 1., 1.));
//	scale       = Mat4::ident().scale(Vec4(4, 1., 1., 2.));
//	rotation     = Mat4::ident().rotate(Vec4(4, 0., 0., 0.), 45.);
//	translation   = Mat4::ident().translated(Vec4(4, -1., -0., 1.));

	cl::CreateLines();
	camera = Camera(
			/*position*/Vec4(4, 0., 1., 1.),
			/*look at*/Vec4(4, 0., 0., 0.),
			/* head */Vec4(4, 0., 1., 0.));

	view       = camera.GetView();
	projection = camera.projectionMatrix(30., 1366./768., 1., 100.);
	model      = Mat4::ident();

	MVP = projection * view * model;

	vertexArrays = new GLuint[1];
	glGenVertexArrays(1, vertexArrays);
	buffer = new GLuint[1];
	glGenBuffers(1, buffer);
	lsProgram.Use();
	lsProgram.GetAttribAllocation("vertexPosition");
	lsProgram.EnableVertexAttribArray();
	lsProgram.GetUniformLocation("trans");
	lsProgram.UniformMatrix(MVP);


	glClearColor(0.2f, 0.3f, 0.4f, 0.0f);

	glutMainLoop();

	return 0;
}

//��������� ������ �������	
///������� ����� ������� � ������ ������
///GL_int shader_obj = glCreateShader, GL_VERTEX_SHADER, GL_PIXEL_SHADER
///glShaderSource(shader_obj, ...)
///glCompileShader -- ������ ������
///int status; glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &status); GL_FALSE => ��� ���������
///glDeleteShader(shader_obj) ���� ��� �����

//��������� ������ ����������
///��. ����

//��������� ��������� (��� ������)
///type? prg_obj = glCreateProgram(void)

//��������� ������� � ��������� ���������
///glAttachShader(prg_obj, shader_obj)
///type? link_status = glLinkProgram(prg_obj)
///glGetProgramiv()

//������� ��������� ��������� � �������

//Render():
//glClear(GL_COLOR_BUFFER_BIT)
//glUseProgram(...)
//glBindVertexArray(...)
//glDrawArrays(GL_TRIANGLES, 0, 3);
//glUse 0, glBind 0
//glutSwapBuffers();

//** glUseProgram(sp)

//* glGenVertexArrays(count, &vao)
//* glBindVertexArray(vao_id)

//glGenBuffers(count, &buf__id)
//glBindBuffer(GL_ARRAY_BUFFER, buf_id)
//glBufferData(GL_ARRAY_BUFFER, sizeOfBytes, &data, GL_STATIC_DRAW)
//GL_int glGetAttribLocation(sp, var_name) ///returns var_id
///sp - createprogram
//glVertexAttribPointer(var_id, count(vec?), GL_FLOAT, GL_FALSE, 0, 0)
//glEnableVertexAttribArray(var_id)
//glBindBuffer(GL_ARRAY_BUFFER, 0)

//* glBindVertexArray(0)

//**glUseProgram(0)

//VBO, VAO


//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
//	Vec4 v = Vec4(2, 5.1, 6.6);
