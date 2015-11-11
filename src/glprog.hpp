#ifndef GLPROG_HPP_
#define GLPROG_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>

class Shader {
public:
	GLuint handler;
	Shader(char *fileName, GLenum shaderType);
};

class Program {
public:
	GLuint handler;
	GLint attribArray;
	Program();
	Program(const Shader &shader1, const Shader &shader2);
	void GetAttribAllocation(const char *var) {
		glGetAttribLocation(handler, var);
	}
	void EnableVertexAttribArray() {
		glEnableVertexAttribArray(attribArray);
	}
	void Use() {
		glUseProgram(handler);
	}
	void VertexAttribPointer(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
		glVertexAttribPointer(attribArray, size, type, normalized, stride, pointer);
	}
};

#endif /* GLPROG_HPP_ */
