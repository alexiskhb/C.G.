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
#include <map>
#include <string>

class Shader {
public:
	GLuint handler;
	Shader(const char *fileName, GLenum shaderType);
};

class Program {
public:
	GLuint handler;
	GLint  attribArray;
	std::map<std::string, int> attribs;
	Program();
	Program(const Shader &shader1, const Shader &shader2);
	void GetAttribAllocation(const char *var) {
		glGetAttribLocation(handler, var);
	}
	int Location(std::string var, bool is_uniform) {
		if (is_uniform) {
			return glGetUniformLocation(handler, var.c_str());
		} else {
			return glGetAttribLocation(handler, var.c_str());
		}
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
	void UniformMatrix(const GLfloat *data, GLuint matrix_handler) {
		glUniformMatrix4fv(matrix_handler, 1, GL_FALSE, data);
	}
	void UniformInt(const GLint data, GLuint int_handler) {
		glUniform1i(int_handler, data);
	}
	void UniformVec(Vec4 pos, GLuint vec_handler) {
		glUniform3f(vec_handler, pos[0], pos[1], pos[2]);
	}
	void AttachShader(const char *fileName, GLenum shaderType) {
		glAttachShader(handler, Shader(fileName, shaderType).handler);
	}
	void Link() {
		glLinkProgram(handler);
	}
};

class Buffer {
public:
	GLuint vbo;
	GLuint vao;
	GLuint via;
	GLint first;
	GLint count;
	GLenum mode;
	Buffer() {
		first = count = mode = vao = vbo = via = 0;
	}
	void Init() {
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &via);
	}
	void Draw(Program prog) {
		glBindVertexArray(vao);
		glDrawArrays(mode, first, count);
		glBindVertexArray(0);
	}
	void DrawElements(Program prog, GLint elem_cnt, unsigned short int *indexes) {
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, via);
		glDrawElements(mode, elem_cnt, GL_UNSIGNED_SHORT, 0);
	}
};

#endif /* GLPROG_HPP_ */
