#include "mat.hpp"

using namespace std;

Mat::Mat(int ht, int wt) {
	height = ht;
	width  = wt;
	data   = new floatv[wt*ht];
	datasize = wt*ht;
	valid  = VTOK;
}

bool Mat::operator==(const Mat &a) {
	bool result = a.height == height && a.width == width;

	return result;
}

Mat::Mat(int ht, int wt, floatv(*func)(int, int, int, int)) : Mat(ht, wt) {
	for(int i = 0; i < ht; i++)
		for(int j = 0; j < wt; j++)
			(*this)(i, j) = func(i, j, ht, wt);
}

Mat::~Mat() {
	//delete [] this->data;
}

Mat Mat::operator+(floatv value) {
	Mat result = Mat(height, width);
	operatorAdd(result, value);
	return result;
}

Mat Mat::operator*(floatv value) {
	Mat result = Mat(height, width);
	operatorMult(result, value);
	return result;
}

Mat Mat::operator*(const Mat &m) {
	Mat result = Mat(height, width);
	operatorMultMatr(result, m);
	return result;
}

float Mat::degreedElSum(int degree) {
	floatv result = 0.0;
	for(int i = 0; i < this->width*this->height; i++)
		result += pow(this->data[i], degree);
	return result;
}

void Mat::transpose() {
	if(height == 1 || width == 1) {
	} else
	if(height != width) {
		floatv *newdata = new float[height*width];
		for(int i = 0; i < height; i++)
			for(int j = 0; j < width; j++)
				newdata[j*height + i] = this->data[i*width + j];
		delete [] this->data;
		this->data = newdata;
	} else {
		for(int i = 0; i < height; i++)
			for(int j = i; j < width; j++)
				swap((*this)(i, j), (*this)(j, i));
	}
	swap(height, width);
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
	if (result.capacity() < this->height*m.width) {
		result.valid = VTBADCAPACITY;
		return;
	}
	for(int i = 0; i < this->height; i++)
		for(int j = 0; j < m.width; j++) {
			result(i, j) = 0.0;
			for(int k = 0; k < this->width; k++) {
				result(i, j) += (*this)(i, k) * m(k, j);
			}
		}
	result.height = this->height;
	result.width  = m.width;
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
	for(int i = 0; i < target.height; i++)
		for(int j = 0; j < target.width; j++)
			target(i, j) = i == j ? 1. : 0.;
}

void Mat::print(ostream &out) {
	for(int i = 0; i < this->height; i++)
		for(int j = 0; j < this->width; j++)
			out << (*this)(i, j) << (j == this->width - 1 ? '\n' : ' ');
}

floatv Mat::det3(int r1, int r2, int r3, int c1, int c2, int c3) {
	Mat a = *this;
	if (max(r1, max(r2, r3)) >= height || max(r1, max(r2, r3)) >= width)
	{
		this->valid = VTOUTOFBORDERS;
		return 0.0;
	}
	return
		a(r1, c1)*a(r2, c2)*a(r3, c3) + a(r1, c2)*a(r2, c3)*a(r3, c1) + a(r1, c3)*a(r2, c1)*a(r3, c2) -
		a(r1, c3)*a(r2, c2)*a(r3, c1) - a(r2, c3)*a(r3, c2)*a(r1, c1) - a(r3, c3)*a(r1, c2)*a(r2, c1);
}

floatv Mat::det2(int r1, int r2, int c1, int c2) {
	Mat a = *this;
	if (max(r1, r2) >= height || max(r1, r2) >= width)
	{
		this->valid = VTOUTOFBORDERS;
		return 0.0;
	}
	return a(r1, c1)*a(r2, c2) - a(r2, c1)*a(r1, c2);
}





