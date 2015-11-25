#ifndef GLPROG_HPP_
#define GLPROG_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <string>
#include "mat4.hpp"
#include "mat.hpp"
#include "vec4.hpp"

class Shader {
public:
	GLuint handler;
	Shader(const char *fileName, GLenum shaderType);
};

class Program {
public:
	GLuint handler;
	GLint  attribArray;
	GLuint mvp_handler;
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
	void GetUniformLocation(const char *univar) {
		mvp_handler = glGetUniformLocation(handler, univar);
	}
	void UniformMatrix(const GLfloat *data) {
		glUniformMatrix4fv(mvp_handler, 1, GL_FALSE, data);
	}
	void FillUniform4fv(const char *univar, const GLfloat *data) {
		GetUniformLocation(univar);
		UniformMatrix(data);
	}
	void AttachShader(const char *fileName, GLenum shaderType) {
		glAttachShader(handler, Shader(fileName, shaderType).handler);
	}
	void Link() {
		glLinkProgram(handler);
	}
};

#endif /* GLPROG_HPP_ */
