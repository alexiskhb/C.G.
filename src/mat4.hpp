#ifndef MAT4_HPP_
#define MAT4_HPP_

#include "mat.hpp"
#include "vec4.hpp"
#include <cmath>

class Mat4 : public Mat {
////
public:
	Mat4() : Mat(4, 4) {};
	Mat4(const Mat4& orig);
	Mat4(ValidityT validity) : Mat(validity) {};
	Mat4(floatv(*func)(int row, int col, int height, int width)) : Mat(4, 4, func) {};
	Mat4(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3);
	Mat4(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3, const Vec4 &v4);
	static Mat4 ident();
	virtual ~Mat4();
////
	Mat4  operator+ (floatv value);
	Mat4  operator+ (const Mat4 &m);
	Mat4& operator+=(const Mat4 &m) {operatorAdd(*this, m,  1);  return *this;};
	Mat4& operator+=(floatv value)  {operatorAdd(*this,  value); return *this;};

	Mat4  operator- (floatv value) {return operator+(-value);};
	Mat4  operator- (const Mat4 &m); Mat4& operator-=(const Mat4 &m) {operatorAdd(*this, m, -1); return *this;};
	Mat4& operator-=(floatv value) {return operator+=(-value);};

	Mat4  operator* (floatv value);
	Mat4  operator* (const Mat4 &m);
	Vec4  operator* (const Vec4 &v);
	Mat4& operator*=(floatv value) {operatorMult(*this, value); return *this;};

	Mat4  operator/ (floatv value) {return fabs(value) > epsilon ? operator*(1.0/value) : Mat4(VTDIVBYZERO);};
////
	floatv det4();
	Mat4 transposed();
	Mat4 inversed3();
	Mat4 inversed();
	Mat4 multMatr3(Mat4 m);
////
	Mat4 perspective(Vec4 v);
	Mat4 translate(Vec4 v);
	Mat4 scale(Vec4 v);
	Mat4 rotate(Vec4 v, floatv angle);
};

#endif
