#ifndef VEC4_HPP_
#define VEC4_HPP_

#include "mat.hpp"
#include "mat4.hpp"
#include <utility>


class Vec4 : public Mat {
public:
	Vec4() : Mat(4, 1) {};
	Vec4(floatv x, floatv y, floatv z) : Mat(4, 1) {X(x); Y(y); Z(z);};
	Vec4(floatv x, floatv y, floatv z, floatv w) : Mat(4, 1) {X(x); Y(y); Z(z); W(w);};
	Vec4(const Vec4& orig);
	Vec4(ValidityT validity) : Mat(validity) {};
	virtual ~Vec4();
////
	inline floatv X() {return (*this)[0];};
	inline floatv Y() {return (*this)[1];};
	inline floatv Z() {return (*this)[2];};
	inline floatv W() {return (*this)[3];};
	inline floatv X(floatv value) {return (*this)[0] = value;};
	inline floatv Y(floatv value) {return (*this)[1] = value;};
	inline floatv Z(floatv value) {return (*this)[2] = value;};
	inline floatv W(floatv value) {return (*this)[3] = value;};
////
	Vec4  operator+ (floatv value);
	Vec4  operator+ (const Vec4 &v);
	Vec4& operator+=(floatv value)  {operatorAdd(*this, value); return *this;};
	Vec4& operator+=(const Vec4 &v) {operatorAdd(*this, v,  1); return *this;};

	Vec4  operator- (floatv value)  {return operator+(-value);};
	Vec4  operator- (const Vec4 &v);
	Vec4& operator-=(floatv value)  {return operator+=(-value);};
	Vec4& operator-=(const Vec4 &v) {operatorAdd(*this, v, -1); return *this;};

	Vec4  operator* (floatv value);
	Mat4  operator* (const Vec4 &v);

	Vec4  operator/ (floatv value) {return fabs(value) > epsilon ? operator*(1.0/value) : Vec4(VTDIVBYZERO);};

	Vec4& operator*=(floatv value) {operatorMult(*this, value); return *this;};

	inline floatv& operator[](int index) {return this->data[index];};
////
	static Vec4   cross3(Vec4 a, Vec4 b);
	static Vec4   bycomp(Vec4 a, Vec4 b);
	static floatv dot3  (Vec4 a, Vec4 b);
	inline floatv length()  {return sqrt(degreedElSum(2));};
	inline floatv length3();
	void   normalize();
	Vec4   normalized();
	Vec4   cross3(Vec4 v) {return cross3(*this, v);};
	floatv dot3  (Vec4 v) {return   dot3(*this, v);};
};

#endif

