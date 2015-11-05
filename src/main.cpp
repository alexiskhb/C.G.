#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include "mat4.hpp"
#include "mat.hpp"
#include "test.hpp"
#include "vec4.hpp"

/*
mat translate(mat, vec) m = tmat*mat, tmat =
 1 0 0 v1
 0 1 0 v2
 0 0 1 v3
 0 0 0  1

mat rotate(mat, vec axis, angle)
mat scale(mat, vec)
then mult rmat*vetrex

mat otrho(left, right, top, bottom, near, far) //cuts
mat frustum(left, right, top, bottom, near, far) //cut pyramide
mat perspective(field_of_view_y(angle), aspect_relative(=w/h), near, far)
r2mat

shader <- rmat*r2mat

struct Camera
float fovy, near, far
int w, h
vec position, targetview, side

  /
  | sidex, sidey, sidez, 0
  | ypx, upy, upz, 0
  | targetx, ty, tz, 0
  | 0 0 0 1
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

const int WT = 640;
const int HT = 480;

char triangleVertexShaderName[] = "../shaders/VerticesShader.hlsl";
char triangleFragmentShaderName[] = "../shaders/FragmentShader.hlsl";
char squareFragmentShaderName[] = "../shaders/SquareFragmentShader.hlsl";

typedef enum {
	FTSQUARE, FTTRGL, FTCNT
} FIGURE_T;

float triangleVertices[] =
{
	0.3f, 1.0f, 0.0f,
	-0.8f, -0.45f, 0.0f, //left
	0.6f, -1.0f, 0.0f
};
float squareVertices[] =
{
	-0.9f, 0.9f, 0.0f, //lu
	-0.1f, 0.9f, 0.0f, //ru
	-0.1f, 0.1f, 0.0f, //rd
	-0.9f, 0.1f, 0.0f  //ld
};

GLint readCompileShader(char *fileName, GLenum shaderType) {
	ifstream fileToRead(fileName);
	if (!fileToRead) {
		cout << "No such file: " << fileName << endl;
		exit(0);
	}
	fileToRead.seekg(0, fileToRead.end);
	const GLint shaderLength[1] = {fileToRead.tellg()};
	fileToRead.seekg(0, fileToRead.beg);
	GLchar *textBuffer = new GLchar[shaderLength[0]];
	fileToRead.read(textBuffer, shaderLength[0]);
	const GLchar *shaderText[1] = {textBuffer};
	GLint shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, shaderText, shaderLength);
	delete [] textBuffer;
	glCompileShader(shaderId);
	GLint is_compiled;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &is_compiled);
	if (!is_compiled) {
		GLint log_length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *info_log = new GLchar[log_length];
		GLint returned_length = 0;
		glGetShaderInfoLog(shaderId, log_length, &returned_length, info_log);
		glDeleteShader(shaderId);
		cout << "Shader compilation error in " << fileName;
		cout << '\n' << info_log;
		delete [] info_log;
		exit(0);
	}
	return shaderId;
}

GLuint triangleShaderProgram;
GLuint squareShaderProgram;
GLuint *vertexArrays;
GLuint squareVertexArray;

GLuint *buffer;

GLint attribArray;

void RenderTriangleAndSquare() {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(triangleShaderProgram);
	//attribArray = glGetAttribLocation(triangleShaderProgram, "vertexPosition");
	glBindVertexArray(vertexArrays[FTTRGL]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FTTRGL]);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), triangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribArray);
	glVertexAttribPointer(attribArray, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(squareShaderProgram);
	//attribArray = glGetAttribLocation(squareShaderProgram, "vertexPosition");
	glBindVertexArray(vertexArrays[FTSQUARE]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[FTSQUARE]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), squareVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribArray);
	glVertexAttribPointer(attribArray, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();
}

GLuint CreateProgram(char *sh1name, GLenum sh1type, char *sh2name, GLenum sh2type) {
	GLint vertexShader = readCompileShader(sh1name, sh1type);
	GLint fragmentShader = readCompileShader(sh2name, sh2type);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	GLint isLinkSuccesful;
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinkSuccesful);
	if (!isLinkSuccesful) {
		GLint log_length = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *info_log = new GLchar[log_length];
		GLint returned_length = 0;
		glGetProgramInfoLog(shaderProgram, log_length, &returned_length, info_log);
		cout << "Program linking error\n" << info_log;
		glDeleteProgram(shaderProgram);
		delete [] info_log;
		exit(0);
	}
	return shaderProgram;
}

float mtr(int i, int j, int h, int w) {
	return fabs(i + j*2);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();


//	Mat4 E = Mat4::createIdent();
//	cout << "E\n" << E << endl;
//	Mat4 M = Mat4(mtr);
//	cout << "M\n" << M << endl << (M + 2) << endl << (M * 2) << endl << (M - 2) << endl << (M + E) << endl << (M * (E*2)) << endl;
//	Mat4 Minv = M.inversed3();

	Vec4 v = Vec4(2, 5.1, 6.6);

//	Vec4 v = Vec4(1, 2, 3);
//	Vec4 u = Vec4(5, 6, 7);
//	Vec4 v1 = Vec4(1, 3, 6), v2 = Vec4(22, 5, 1), v3 = Vec4(64, 45, 0, 9), v4 = Vec4(1, 32, 2, 1);

//	u.transpose();
//	cout << u << endl << v << endl;
//	Mat4 m = u*v;
//	cout << m << endl;
	return 0;

	glutInit(&argc, argv);
	glutInitWindowSize(WT, HT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(CAPTION);
	glutDisplayFunc(RenderTriangleAndSquare);
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
		return 1;

	triangleShaderProgram = CreateProgram(
		triangleVertexShaderName, GL_VERTEX_SHADER,
		triangleFragmentShaderName, GL_FRAGMENT_SHADER);

	squareShaderProgram = CreateProgram(
		triangleVertexShaderName, GL_VERTEX_SHADER,
		squareFragmentShaderName, GL_FRAGMENT_SHADER);

	vertexArrays = new GLuint[FTCNT];
	glGenVertexArrays(FTCNT, vertexArrays);
	buffer = new GLuint[FTCNT];
	glGenBuffers(FTCNT, buffer);
	attribArray = glGetAttribLocation(triangleShaderProgram, "vertexPosition");
	glEnableVertexAttribArray(attribArray);

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
