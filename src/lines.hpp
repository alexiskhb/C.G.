#ifndef LINES_HPP_
#define LINES_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mat.hpp"
#include "glprog.hpp"

class Lines {
public:
	float *lines;
	int lamt;
	GLuint vertexArray;
	GLuint buffer;
	Lines(int amt) {
		lines = new float[amt*2*3];
		this->lamt = amt;
		vertexArray = 0;
		buffer = 0;
	}
	~Lines();
	void SetLine(int st, floatv x1, floatv z1, floatv x2, floatv z2);
	void SetLine(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2);
	void Draw(Program prog);
};

#endif

