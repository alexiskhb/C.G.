#include "mat.hpp"

using namespace std;

Mat::Mat(int height, int width) {
	this->height = height;
	this->width  = width;
	this->data   = new floatv[width*height];
	this->valid  = VTOK;
}

Mat::Mat(int height, int width, floatv(*func)(int, int)) : Mat(height, width) {
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			(*this)(i, j) = func(i, j);
}

Mat::~Mat() {
	//delete [] this->data;
}

float Mat::degreedElSum(int degree) {
	floatv result = 0.0;
	for(int i = 0; i < this->width*this->height; i++)
		result += pow(this->data[i], degree);
	return result;
}

void Mat::transpose() {
	floatv *newdata = new float[height*width];
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
			newdata[j*height + i] = this->data[i*width + j];
	swap(height, width);
	delete [] this->data;
	this->data = newdata;
}

void Mat::operatorAdd(Mat &result, floatv value) {
	if (!isValid()) {
		result.valid = VTERR;
		return;
	}
	for(int i = 0; i < result.width*result.height; i++)
		result(0, i) = (*this)(0, i) + value;
}

void Mat::operatorMult(Mat &result, floatv value) {
	if (!isValid()) {
		result.valid = VTERR;
		return;
	}
	for(int i = 0; i < result.width*result.height; i++)
		result(0, i) = (*this)(0, i) * value;
}

void Mat::operatorMultMatr(Mat &result, Mat m) {
	if (!isValid() || !m.isValid()) {
		result.valid = VTERR;
		return;
	}
	if (this->width != m.height) {
		result.valid = VTBADMULTIPLY;
		return;
	}
	for(int i = 0; i < result.height; i++)
		for(int j = 0; j < result.width; j++) {
			result(i, j) = 0.0;
			for(int k = 0; k < this->width; k++) {
				result(i, j) += (*this)(i, k) * m(k, j);
			}
		}
}

void Mat::operatorAdd(Mat &result, Mat m, int sign) {
	if (!isValid() || !m.isValid()) {
		result.valid = VTERR;
		return;
	}
	for(int i = 0; i < min(result.height, min(m.height, this->height)); i++)
		for(int j  = 0; j < min(result.width, min(m.width, this->width)); j++)
			result(i, j) = (*this)(i, j) + sign*m(i, j);
}

void Mat::makeIdent(Mat &target) {
	for(int i = 0; i < min(target.height, target.width); i++)
		target(i, i) = 1.0;
}

void Mat::print(ostream &out) {
	for(int i = 0; i < this->height; i++)
		for(int j = 0; j < this->width; j++)
			out << (*this)(i, j) << (j == this->width - 1 ? '\n' : ' ');
}
