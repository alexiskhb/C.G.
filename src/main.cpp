#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>

#define CAPTION ("Shch")

using namespace std;

const int WT = 640;
const int HT = 480;

char triangleVertexShaderName[] = "./shaders/VerticesShader.hlsl";
char triangleFragmentShaderName[] = "./shaders/FragmentShader.hlsl";

GLint readCompileShader(char *fileName, GLenum shaderType)
{
	ifstream fileToRead(fileName);
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
	if (!is_compiled)
	{
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
GLuint triangleVertexArray;

GLuint squareShaderProgram;
GLuint squareVertexArray;

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(triangleShaderProgram);
	glBindVertexArray(triangleVertexArray);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);
	glBindVertexArray(0);
	glutSwapBuffers();
}

GLuint CreateProgram(char *sh1name, GLenum sh1type, char *sh2name, GLenum sh2type)
{
	GLint vertexShader = readCompileShader(sh1name, sh1type);
	GLint fragmentShader = readCompileShader(sh2name, sh2type);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	GLint isLinkSuccesful;
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinkSuccesful);
	if (!isLinkSuccesful)
	{
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

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(WT, HT);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(CAPTION);
	glutDisplayFunc(Render);
	//glutPassiveMotionFunc(MouseMove);
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
	if (res != GLEW_OK)
		return 1;

	float triangleVertices[] =
	{
		0.3f, 1.0f, 0.0f,
		-0.8f, -0.45f, 0.0f, //left
		0.6f, -1.0f, 0.0f
	};

	float squareVertices[] =
	{
		-0.9f, -0.9f, 0.0f, //lu
		-0.1f, -0.9f, 0.0f, //ru
		-0.1f, -1.0f, 0.0f, //rd
		-0.9f, -0.1f, 0.0f  //ld
	};

	triangleShaderProgram = CreateProgram(
		triangleVertexShaderName, GL_VERTEX_SHADER,
		triangleFragmentShaderName, GL_FRAGMENT_SHADER);

	glGenVertexArrays(1, &triangleVertexArray);
	glBindVertexArray(triangleVertexArray);

	GLuint buffer = 0;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), triangleVertices, GL_STATIC_DRAW);

	GLint attribArray = glGetAttribLocation(triangleShaderProgram, "vertexPosition");
	glEnableVertexAttribArray(attribArray);
	glVertexAttribPointer(attribArray, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
