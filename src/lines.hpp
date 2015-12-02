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

struct Point3 {
	float c[3];
	Point3() {
		c[0] = c[1] = c[2] = 0;
	}
	Point3(float _x, float _y, float _z) {
		c[0] = _x;
		c[1] = _y;
		c[2] = _z;
	}
	float x() {return c[0];};
	float y() {return c[1];};
	float z() {return c[2];};
};

class Triangle {
public:
	Point3 points[3];
	Point3 normals[3];
	Triangle(){};
	Triangle(Point3 a, Point3 b, Point3 c) {
		points[0] = a;
		points[1] = b;
		points[2] = c;
		Vec4 va(4, a.x(), a.y(), a.z());
		Vec4 vb(4, b.x(), b.y(), b.z());
		Vec4 vc(4, c.x(), c.y(), c.z());
		Vec4 ca = va - vc;
		Vec4 ab = vb - va;
		Vec4 bc = vc - vb;
		Vec4 na = Vec4::cross3(ca, ab);
		Vec4 nb = Vec4::cross3(ab, bc);
		Vec4 nc = Vec4::cross3(bc, ca);
		normals[0] = Point3(na[0], na[1], na[2]);
		normals[1] = Point3(nb[0], nb[1], nb[2]);
		normals[2] = Point3(nc[0], nc[1], nc[2]);
	}
};

class Plane {
public:
	float vertex_normals[2*4*3];
	unsigned short int indexes[6] = {0, 1, 2, 0, 2, 3};
	Plane() {}
	Plane(Vec4 cen, float side) : Plane(
			Point3(cen[0]-side/2., cen[1], cen[2]-side/2.),
			Point3(cen[0]+side/2., cen[1], cen[2]-side/2.),
			Point3(cen[0]+side/2., cen[1], cen[2]+side/2.),
			Point3(cen[0]-side/2., cen[1], cen[2]+side/2.)
			) {
	};
	Plane(Point3 a, Point3 b, Point3 c, Point3 d) {
		Triangle t1(a, b, c);
		Triangle t2(a, c, d);
		Point3 points[4] = {t1.points[0], t1.points[1], t1.points[2], t2.points[2]};
		Point3 normals[4] = {t1.normals[0], t1.normals[1], t1.normals[2], t2.normals[2]};
		for(int v = 0, n = 12, i = 0; i < 4; i++)
			for(int j = 0; j < 3; j++, v++, n++) {
				vertex_normals[v] = points[i].c[j];
				vertex_normals[n] = normals[i].c[j];
			}
	}
	void FillBuffer(Buffer *buff, Program prog);
	void FillIndexBuffer(Buffer *buff, Program prog);
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
	Cube(){};
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

