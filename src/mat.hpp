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
	VTOK, VTBADMULTIPLY, VTDIVBYZERO, VTERR
} ValidityT;

class Mat {
public:
	int width;
	int height;
	ValidityT valid;
//------
	Mat(int width, int height);
	Mat(ValidityT validity) : Mat(0, 0) {this->valid = validity;};
	Mat(int height, int width, floatv(*)(int, int));
	virtual ~Mat();
//------
	bool isValid() {return this->valid == VTOK;};
	void print(std::ostream &out);
	void transpose();
	floatv& operator()(int i, int j) {return this->data[i*this->width + j];};
//------
protected:
	floatv *data;
//------
	floatv degreedElSum(int degree);
	void operatorAdd(Mat &result, floatv value);
	void operatorSub(Mat &result, floatv value) {operatorAdd(result, -value);};
	void operatorMult(Mat &result, floatv value);
	void operatorAdd(Mat &result, Mat m, int sign);
	void operatorMultMatr(Mat &result, Mat m);
//------
	static void makeIdent(Mat &target);
//------
};

#endif
