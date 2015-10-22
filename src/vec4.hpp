#ifndef VEC4_HPP_
#define VEC4_HPP_

#include "mat.hpp"

class Vec4 : public Mat {
public:
	Vec4();
	virtual ~Vec4();
	inline float Length() {return sqrt(DegreedElSum(2));};
	inline float x() {return this->data[0];};
	inline float y() {return this->data[1];};
	inline float z() {return this->data[2];};
	inline float w() {return this->data[3];};
	inline float x(float value) {return this->data[0] = value;};
	inline float y(float value) {return this->data[1] = value;};
	inline float z(float value) {return this->data[2] = value;};
	inline float w(float value) {return this->data[3] = value;};
	Vec4 operator*(Vec4 v);
	inline float& operator[](int index) {return this->data[index];};
};

#endif

