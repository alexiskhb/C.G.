#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <functional>
#include <utility>

typedef float floatv;

const floatv epsilon = 1e-10;

typedef enum {
	VTOK, VTBADMULTIPLY, VTDIVBYZERO, VTERR, VTBADCAPACITY, VTOUTOFBORDERS, VTSINGULAR, VTDIFFSIZE
} ValidityT;

class Mat {
public:
	int width;
	int height;
	ValidityT valid;
////
	Mat(int width, int height);
	Mat(ValidityT validity) : Mat(0, 0) {this->valid = validity;};
	Mat(int height, int width, floatv(*)(int row, int col, int height, int width));
	virtual ~Mat();
////
	int  capacity() {return datasize;};
	bool isValid()  {return this->valid == VTOK;};
	bool operator==(const Mat &a);
	void transpose();
	void print(std::ostream &out);
	floatv& operator()(int i, int j) {return this->data[i*this->width + j];};
	floatv  degreedElSum(int degree);
	floatv  det3(int r1, int r2, int r3, int c1, int c2, int c3);
	floatv  det3() {return Mat::det3(0, 1, 2, 0, 1, 2);};
	floatv  det2(int r1, int r2, int c1, int c2);
	friend  std::ostream& operator<<(std::ostream &out, Mat m) {m.print(out); return out;};
////
	Mat operator+(floatv value);

	Mat operator*(const Mat &m);
	Mat operator*(floatv value);
////
	floatv data[100];
	int datasize;
////
	void operatorAdd(Mat &result, floatv value);
	void operatorSub(Mat &result, floatv value) {operatorAdd(result, -value);};
	void operatorAdd(Mat &result, Mat m, int sign);
	void operatorMult(Mat &result, floatv value);
	void operatorMultMatr(Mat &result, Mat m);
////
	static void makeIdent(Mat &target);
////
};

class Vec4;
class Mat4;

#endif
