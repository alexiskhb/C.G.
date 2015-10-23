#include "mat.hpp"

Mat::Mat(int height, int width) {
	this->height = height;
	this->width  = width;
	this->data   = new float[width*height];
	this->valid  = VTOK;
}

Mat::Mat(ValidityT validity) {
	this->height = 0;
	this->width  = 0;
	this->data   = NULL;
	this->valid  = validity;
}

Mat::~Mat() {
	delete [] this->data;
}

float Mat::degreedElSum(int degree) {
	float result = 0.0;
	for(int i = 0; i < this->width*this->height; i++)
		result += pow(this->data[i], degree);
	return result;
}

void Mat::operatorAdd(Mat *result, float value) {
	if (!isValid()) {
		result->valid = VTERR;
		return;
	}
	for(int i = 0; i < result->width*result->height; i++)
		(*result)(0, i) = (*this)(0, i) + value;
}

void Mat::operatorMult(Mat *result, float value) {
	if (!isValid()) {
		result->valid = VTERR;
		return;
	}
	for(int i = 0; i < result->width*result->height; i++)
		(*result)(0, i) = (*this)(0, i) * value;
}

Mat Mat::createIdent(int size) {
	Mat result = Mat(size, size);
	for(int i = 0; i < size; i++)
		result(i, i) = 1;
	return result;
}

void Mat::print(std::ostream& out) {
	for(int i = 0; i < this->height; i++)
		for(int j = 0; j < this->width; j++)
			out << (*this)(i, j) << (j == this->width - 1 ? '\n' : ' ');
}
