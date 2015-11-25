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
	GLuint vertexArray;
	GLuint buffer;
	GLenum mode;
	Lines(int amt) {
		vertex = new float[amt*3*2];
		this->vamt = amt*2;
		vertexArray = 0;
		buffer = 0;
		mode = GL_LINES;
	}
	~Lines();
	void Init() {
		glGenVertexArrays(1, &vertexArray);
		glGenBuffers(1, &buffer);
	}
	void SetLine(int st, floatv x1, floatv z1, floatv x2, floatv z2);
	void SetLine(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2);
	void Draw(Program prog);
};

class Triangles : public Lines {
public:
	Triangles(int vertexamt) : Lines(vertexamt) {
		mode = GL_TRIANGLES;
	};
	void SetTriangle(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2, floatv x3, floatv y3, floatv z3);
};

class Cube : public Triangles {
public:
	Cube(floatv side, Vec4 center) : Triangles(6*6) {
		floatv a = side/2;
		floatv x = center[0], y = center[1], z = center[2];

		SetTriangle(0, x-a, y-a, z-a, x+a, y-a, z-a, x+a, y+a, z-a);
		SetTriangle(1, x+a, y+a, z-a, x-a, y+a, z-a, x-a, y-a, z-a);

		SetTriangle(2, x-a, y-a, z+a, x+a, y-a, z+a, x+a, y+a, z+a);
		SetTriangle(3, x+a, y+a, z+a, x-a, y+a, z+a, x-a, y-a, z+a);

		SetTriangle(4, x-a, y+a, z+a, x-a, y+a, z-a, x-a, y-a, z-a);
		SetTriangle(5, x-a, y-a, z-a, x-a, y-a, z+a, x-a, y+a, z+a);

		SetTriangle(6, x+a, y+a, z+a, x+a, y+a, z-a, x+a, y-a, z-a);
		SetTriangle(7, x+a, y-a, z-a, x+a, y-a, z+a, x+a, y+a, z+a);

		SetTriangle(8, x-a, y-a, z-a, x+a, y-a, z-a, x+a, y-a, z+a);
		SetTriangle(9, x+a, y-a, z+a, x-a, y-a, z+a, x-a, y-a, z-a);

		SetTriangle(10, x-a, y+a, z-a, x+a, y+a, z-a, x+a, y+a, z+a);
		SetTriangle(11, x+a, y+a, z+a, x-a, y+a, z+a, x-a, y+a, z-a);
	}
};

#endif

