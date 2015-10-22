#ifndef MAT_HPP_
#define MAT_HPP_

#include <iostream>
#include <cmath>

class Mat {
public:
	Mat(int width, int height);
	virtual ~Mat();
protected:
	int width;
	int height;
	float *data;
	float DegreedElSum(int degree);
};

#endif
