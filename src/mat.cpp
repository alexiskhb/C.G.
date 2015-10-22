#include "mat.hpp"

Mat::Mat(int height, int width) {
	this->height = height;
	this->width  = width;
	this->data   = new float[width*height];
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
