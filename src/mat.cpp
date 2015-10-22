#include "mat.hpp"

Mat::Mat(int height, int width) {
	this->height = height;
	this->width  = width;
	this->data   = new float[width*height];
	this->valid = VTOK;
}

Mat::~Mat() {
	delete [] this->data;
}

float Mat::DegreedElSum(int degree) {
	float result = 0.0;
	for(int i = 0; i < this->width*this->height; i++)
		result += pow(this->data[i], degree);
	return result;
}

Mat Mat::operator+(float value) {
	Mat result = Mat(this->height, this->width);
	if (this->valid != VTOK) {
		result.valid = VTERR;
		return result;
	}
	for(int i = 0; i < this->width*this->height; i++)
		this->data[i] += value;
	return result;
}

Mat Mat::operator*(float value) {
	Mat result = Mat(this->height, this->width);
	if (this->valid != VTOK) {
		result.valid = VTERR;
		return result;
	}
	for(int i = 0; i < this->width*this->height; i++)
		this->data[i] *= value;
	return result;
}

Mat Mat::operator*(Mat m) {
	Mat result = Mat(this->height, m.width);
	if (this->valid != VTOK || m.valid != VTOK) {
		result.valid = VTERR;
		return result;
	}
	if (this->width != m.height) {
		result.valid = VTBADMULTIPLY;
		return result;
	}

	return result;
}
