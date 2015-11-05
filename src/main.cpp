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

char fragmentShaderName[] = "../shaders/FragmentShader.hlsl";
char linesShaderName[] = "../shaders/LinesVertices.hlsl";

GLuint linesShaderProgram;
GLuint *vertexArrays;
GLuint *buffer;
GLint attribArray;

namespace prog {

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

}

const int h_lines = 20, v_lines = 20;
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
			SetLine(    i*6, -1.,  1./h_lines*i, 1.,  1./h_lines*i);
			SetLine((i+1)*6, -1., -1./h_lines*i, 1., -1./h_lines*i);
		}
		for(int i = h_lines; i < h_lines + v_lines; i += 2) {
			SetLine(    i*6,  1./v_lines*(i - h_lines), 1.,  1./v_lines*(i - h_lines), -1.);
			SetLine((i+1)*6, -1./v_lines*(i - h_lines), 1., -1./v_lines*(i - h_lines), -1.);
		}
	}
}

void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(linesShaderProgram);
	//attribArray = glGetAttribLocation(triangleShaderProgram, "vertexPosition");
	glBindVertexArray(vertexArrays[0]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, (h_lines*2*3 + v_lines*2*3) * sizeof(float), lines, GL_STATIC_DRAW);
	glEnableVertexAttribArray(attribArray);
	glVertexAttribPointer(attribArray, 3, GL_FLOAT, GL_FALSE, 0, 0);
	for(int i = 0; i < h_lines + v_lines; i++)
		glDrawArrays(GL_LINES, i*2, 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();
}


float mtr(int i, int j, int h, int w) {
	return fabs(i + j*2);
}

int main(int argc, char** argv) {
	cl::CreateLines();
	//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
//	Vec4 v = Vec4(2, 5.1, 6.6);

	glutInit(&argc, argv);
	glutInitWindowSize(WT, HT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(CAPTION);
	glutDisplayFunc(Render);
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
		return 1;

	linesShaderProgram = prog::CreateProgram(
		linesShaderName, GL_VERTEX_SHADER,
		fragmentShaderName, GL_FRAGMENT_SHADER);

	vertexArrays = new GLuint[1];
	glGenVertexArrays(1, vertexArrays);
	buffer = new GLuint[1];
	glGenBuffers(1, buffer);
	attribArray = glGetAttribLocation(linesShaderProgram, "vertexPosition");
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
