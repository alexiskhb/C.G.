#ifndef VEC4_HPP_
#define VEC4_HPP_

#include "mat.hpp"

class Vec4 : public Mat {
public:
	Vec4() : Mat(4, 1) {};
	Vec4(float x, float y, float z) : Mat(4, 1) {X(x); Y(y); Z(z);};
	Vec4(const Vec4& orig);
	Vec4(ValidityT validity) : Mat(validity) {};
	virtual ~Vec4();
//------
	inline float X() {return this->data[0];};
	inline float Y() {return this->data[1];};
	inline float Z() {return this->data[2];};
	inline float W() {return this->data[3];};
	inline float X(float value) {return this->data[0] = value;};
	inline float Y(float value) {return this->data[1] = value;};
	inline float Z(float value) {return this->data[2] = value;};
	inline float W(float value) {return this->data[3] = value;};
//------
	Vec4 operator+(float value);
	Vec4 operator-(float value) {return operator+(-value);};
	Vec4 operator*(float value);
	Vec4 operator/(float value) {return fabs(value) > epsilon ? operator*(1.0/value) : Vec4(VTDIVBYZERO);};
	Vec4 operator*(Vec4 v);
	inline float& operator[](int index) {return this->data[index];};
//------
	inline float length() {return sqrt(degreedElSum(2));};
	void normalize();
	Vec4 normalized();
};

#endif

