#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <cmath>
#include <fstream>

#define epsilon (0.00000001)

typedef enum {
	VTOK, VTBADMULTIPLY, VTDIVBYZERO, VTERR
} ValidityT;

class Mat {
protected:
	float *data;
//------
	float degreedElSum(int degree);
	void operatorAdd(Mat *result, float value);
	void operatorSub(Mat *result, float value) {operatorAdd(result, -value);};
	void operatorMult(Mat *result, float value);
public:
	int width;
	int height;
	ValidityT valid;
//------
	Mat(int width, int height);
	Mat(ValidityT validity);
	static Mat createIdent(int size);
	virtual ~Mat();
//------
	bool isValid() {return this->valid == VTOK;};
	void print(std::ostream& out);
	float& operator()(int i, int j) {return this->data[i*this->width + j];};
};

#endif
