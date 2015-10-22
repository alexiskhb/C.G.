#ifndef VEC4_HPP_
#define VEC4_HPP_

#include "mat.hpp"

class Vec4 : public Mat {
public:
	Vec4();
	virtual ~Vec4();
	float Length();
	float x();
	float x(float value);
	float y();
	float y(float value);
	float z();
	float z(float value);
	float w();
	float w(float value);
};

#endif

