#ifndef LINES_HPP_
#define LINES_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mat.hpp"
#include "vec4.hpp"
#include "mat4.hpp"
#include "glprog.hpp"

class Lines {
public:
	float *vertex;
	int vamt;
	GLenum mode;
	Lines(int amt) {
		vertex = new float[amt*3*2];
		this->vamt = amt*2;
		mode = GL_LINES;
	}
	~Lines();
	void SetLine(int st, floatv x1, floatv z1, floatv x2, floatv z2);
	void SetLine(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2);
	void FillBuffer(Buffer *buff, Program prog);
};

class Cube{
public:
	int vamt = 36;
	float vertex[2*6*6*3];
	unsigned short int indexes[36];
	GLenum mode = GL_TRIANGLES;
	void SetTriangle(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2, floatv x3, floatv y3, floatv z3);
	void FillBuffer(Buffer *buff, Program prog);
	void FillIndexBuffer(Buffer *buff, Program prog);
	Cube(floatv side, Vec4 center){
		floatv a = side/2;
		floatv x = center[0], y = center[1], z = center[2];

		SetTriangle(0, x-a, y-a, z-a, x+a, y-a, z-a, x+a, y+a, z-a);
		SetTriangle(12, 0, 0, -1, 0, 0, -1, 0, 0, -1);
		SetTriangle(1, x+a, y+a, z-a, x-a, y+a, z-a, x-a, y-a, z-a);
		SetTriangle(13, 0, 0, -1, 0, 0, -1, 0, 0, -1);

		SetTriangle(2, x-a, y-a, z+a, x+a, y-a, z+a, x+a, y+a, z+a);
		SetTriangle(14, 0, 0, 1, 0, 0, 1, 0, 0, 1);
		SetTriangle(3, x+a, y+a, z+a, x-a, y+a, z+a, x-a, y-a, z+a);
		SetTriangle(15, 0, 0, 1, 0, 0, 1, 0, 0, 1);


		SetTriangle(4, x-a, y+a, z+a, x-a, y+a, z-a, x-a, y-a, z-a);
		SetTriangle(16, 1, 0, 0, 1, 0, 0, 1, 0, 0);
		SetTriangle(5, x-a, y-a, z-a, x-a, y-a, z+a, x-a, y+a, z+a);
		SetTriangle(17, 1, 0, 0, 1, 0, 0, 1, 0, 0);

		SetTriangle(6, x+a, y+a, z+a, x+a, y+a, z-a, x+a, y-a, z-a);
		SetTriangle(18, -1, 0, 0, -1, 0, 0, -1, 0, 0);
		SetTriangle(7, x+a, y-a, z-a, x+a, y-a, z+a, x+a, y+a, z+a);
		SetTriangle(19, -1, 0, 0, -1, 0, 0, -1, 0, 0);


		SetTriangle(8, x-a, y-a, z-a, x+a, y-a, z-a, x+a, y-a, z+a);
		SetTriangle(20, 0, -1, 0, 0, -1, 0, 0, -1, 0);
		SetTriangle(9, x+a, y-a, z+a, x-a, y-a, z+a, x-a, y-a, z-a);
		SetTriangle(21, 0, -1, 0, 0, -1, 0, 0, -1, 0);

		SetTriangle(10, x-a, y+a, z-a, x+a, y+a, z-a, x+a, y+a, z+a);
		SetTriangle(22, 0, 1, 0, 0, 1, 0, 0, 1, 0);
		SetTriangle(11, x+a, y+a, z+a, x-a, y+a, z+a, x-a, y+a, z-a);
		SetTriangle(23, 0, 1, 0, 0, 1, 0, 0, 1, 0);
	}
};

#endif

