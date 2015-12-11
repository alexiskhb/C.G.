#ifndef LINES_HPP_
#define LINES_HPP_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "mat.hpp"
#include "vec4.hpp"
#include "mat4.hpp"
#include "glprog.hpp"
#include <sstream>

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
	float vertex_normals[2*4*3 + 8];
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
		char v[] = "11010010";
		for(int i = 24, k = 0; i < 32; i += 2) {
			vertex_normals[i    ] = v[k++];
			vertex_normals[i + 1] = v[k++];
		}
	}
	void FillBuffer(Buffer *buff, Program prog);
	void FillIndexBuffer(Buffer *buff, Program prog);
};

struct LandPoint {
	int x;
	int y;
	int z;
};

class Landscape {
	static const int size = 100;
	unsigned int indexes[];
	LandPoint map[size][size];
};

class Cube{
public:
	int vamt = 36;
	float vertex[4*6*3*2 + 2*6*4];
	unsigned short int indexes[36];
	GLenum mode = GL_TRIANGLES;
	void SetEdge(int st, floatv x1, floatv y1, floatv z1, floatv x2, floatv y2, floatv z2, floatv x3, floatv y3, floatv z3, floatv x4, floatv y4, floatv z4);
	void FillBuffer(Buffer *buff, Program prog);
	void FillIndexBuffer(Buffer *buff, Program prog);
	Cube(){};
	Cube(floatv side, Vec4 center){
		floatv a = side/2;
		floatv x = center[0], y = center[1], z = center[2];
#define V0 x-a, y-a, z-a
#define V1 x+a, y-a, z-a
#define V2 x+a, y+a, z-a
#define V3 x-a, y+a, z-a
#define V4 x-a, y-a, z+a
#define V5 x+a, y-a, z+a
#define V6 x+a, y+a, z+a
#define V7 x-a, y+a, z+a
#define N0  0,  0, -1
#define N1 -1,  0,  0
#define N2  0,  0,  1
#define N3  1,  0,  0
#define N4  0, -1,  0
#define N5  0,  1,  0
#define T0 0, 0
#define T1 1, 0
#define T2 1, 1
#define T3 0, 1

		SetEdge(0, V0, V1, V2, V3);
		SetEdge(1, V3, V0, V4, V7);
		SetEdge(2, V7, V4, V5, V6);
		SetEdge(3, V6, V2, V1, V5);
		SetEdge(4, V5, V4, V0, V1);
		SetEdge(5, V2, V3, V7, V6);

		SetEdge(6,  N0, N0, N0, N0);
		SetEdge(7,  N1, N1, N1, N1);
		SetEdge(8,  N2, N2, N2, N2);
		SetEdge(9,  N3, N3, N3, N3);
		SetEdge(10, N4, N4, N4, N4);
		SetEdge(11, N5, N5, N5, N5);

		SetEdge(12, T0, T1, T2, T3, T2, T1);
		SetEdge(13, T0, T3, T2, T1, T0, T3);
		SetEdge(14, T2, T3, T0, T1, T2, T1);
		SetEdge(15, T0, T3, T0, T3, T2, T1);
	}
};

#endif

