#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <functional>
#include <utility>

typedef float floatv;

const floatv epsilon = 1e-7;

typedef enum {
	VTOK, VTBADMULTIPLY, VTDIVBYZERO, VTERR, VTBADCAPACITY, VTOUTOFBORDERS
} ValidityT;

class Mat {
public:
	int width;
	int height;
	ValidityT valid;
//------
	Mat(int width, int height);
	Mat(ValidityT validity) : Mat(0, 0) {this->valid = validity;};
	Mat(int height, int width, floatv(*)(int row, int col, int height, int width));
	virtual ~Mat();
//------
	int  capacity() {return datasize;};
	bool isValid()  {return this->valid == VTOK;};
	void print(std::ostream &out);
	void transpose();
	floatv& operator()(int i, int j) {return this->data[i*this->width + j];};
	floatv  degreedElSum(int degree);
	floatv det3(int r1, int r2, int r3, int c1, int c2, int c3);
	floatv det2(int r1, int r2, int c1, int c2);
//------
protected:
	floatv *data;
	int datasize;
//------
	void operatorAdd(Mat &result, floatv value);
	void operatorSub(Mat &result, floatv value) {operatorAdd(result, -value);};
	void operatorAdd(Mat &result, Mat m, int sign);
	void operatorMult(Mat &result, floatv value);
	void operatorMultMatr(Mat &result, Mat m);
//------
	static void makeIdent(Mat &target);
//------
};

class Mat4;
class Vec4;

#endif
