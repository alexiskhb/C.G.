#ifndef VEC4_HPP_
#define VEC4_HPP_

#include "mat.hpp"
#include <utility>

using namespace std;

class Vec4 : public Mat {
//------
public:
	Vec4() : Mat(4, 1) {};
	Vec4(floatv x, floatv y, floatv z) : Mat(4, 1) {X(x); Y(y); Z(z);};
	Vec4(const Vec4& orig);
	Vec4(ValidityT validity) : Mat(validity) {};
	virtual ~Vec4();
//------
	inline floatv X() {return (*this)[0];};
	inline floatv Y() {return (*this)[1];};
	inline floatv Z() {return (*this)[2];};
	inline floatv W() {return (*this)[3];};
	inline floatv X(floatv value) {return (*this)[0] = value;};
	inline floatv Y(floatv value) {return (*this)[1] = value;};
	inline floatv Z(floatv value) {return (*this)[2] = value;};
	inline floatv W(floatv value) {return (*this)[3] = value;};
//------
	Vec4 operator+(floatv value);
	Vec4 operator-(floatv value) {return operator+(-value);};
	Vec4 operator*(floatv value);
	Vec4 operator/(floatv value) {return fabs(value) > epsilon ? operator*(1.0/value) : Vec4(VTDIVBYZERO);};
	Vec4 operator+(Vec4 v);
	Vec4 operator-(Vec4 v);
	Vec4 operator*(Vec4 v);
	inline floatv& operator[](int index) {return this->data[index];};
//------
	inline floatv length() {return sqrt(degreedElSum(2));};
	void normalize();
	void transpose() {swap(height, width);};
	Vec4 normalized();
};

#endif

