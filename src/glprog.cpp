#include "glprog.hpp"

using namespace std;

Shader::Shader(char *fileName, GLenum aShaderType) {
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
	handler = glCreateShader(aShaderType);
	glShaderSource(handler, 1, shaderText, shaderLength);
	delete [] textBuffer;
	glCompileShader(handler);
	GLint is_compiled;
	glGetShaderiv(handler, GL_COMPILE_STATUS, &is_compiled);
	if (!is_compiled) {
		GLint log_length = 0;
		glGetShaderiv(handler, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *info_log = new GLchar[log_length];
		GLint returned_length = 0;
		glGetShaderInfoLog(handler, log_length, &returned_length, info_log);
		glDeleteShader(handler);
		cout << "Shader compilation error in " << fileName;
		cout << '\n' << info_log;
		delete [] info_log;
		exit(0);
	}

}

Program::Program() {
	handler = 0;
	attribArray = 0;
};

Program::Program(const Shader &shader1, const Shader &shader2) {
	handler = glCreateProgram();
	attribArray = 0;
	glAttachShader(handler, shader1.handler);
	glAttachShader(handler, shader2.handler);
	GLint isLinkSuccesful;
	glLinkProgram(handler);
	glGetProgramiv(handler, GL_LINK_STATUS, &isLinkSuccesful);
	if (!isLinkSuccesful) {
		GLint log_length = 0;
		glGetProgramiv(handler, GL_INFO_LOG_LENGTH, &log_length);
		GLchar *info_log = new GLchar[log_length];
		GLint returned_length = 0;
		glGetProgramInfoLog(handler, log_length, &returned_length, info_log);
		cout << "Program linking error\n" << info_log;
		glDeleteProgram(handler);
		delete [] info_log;
		exit(0);
	}
}
