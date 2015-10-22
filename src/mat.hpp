#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <cmath>

typedef enum {
	VTOK, VTBADMULTIPLY, VTERR
} ValidityT;

class Mat {
protected:
	int width;
	int height;
	float *data;
	float DegreedElSum(int degree);
public:
	ValidityT valid;
	Mat(int width, int height);
	virtual ~Mat();
	Mat operator+(float value);
	Mat operator-(float value) {return operator+(-value);};
	Mat operator*(float value);
	Mat operator/(float value) {return operator*(static_cast<float>(1.0/value));};
	Mat operator+(int value) {return operator+(static_cast<float>(value));};
	Mat operator-(int value) {return operator+(static_cast<float>(-value));};
	Mat operator*(int value) {return operator*(static_cast<float>(-value));};
	Mat operator/(int value) {return operator*(static_cast<float>(1.0/value));};
	Mat operator*(Mat m);
	float operator()(int i, int j) {return this->data[i*this->width + j];};
};

#endif
