#ifndef MAT4_HPP_
#define MAT4_HPP_

#include "mat.hpp"
#include "vec4.hpp"

class Mat4 : public Mat {
//------
public:
	Mat4() : Mat(4, 4) {};
	Mat4(const Mat4& orig);
	Mat4(ValidityT validity) : Mat(validity) {};
	Mat4(floatv(*func)(int row, int col, int height, int width)) : Mat(4, 4, func) {};
	static Mat4 createIdent();
	virtual ~Mat4();
//------
	Mat4 operator+(floatv value);
	Mat4 operator-(floatv value) {return operator+(-value);};
	Mat4 operator*(floatv value);
	Mat4 operator/(floatv value) {return fabs(value) > epsilon ? operator*(1.0/value) : Mat4(VTDIVBYZERO);};
	Mat4 operator*(Mat4 m);
	Mat4 operator+(Mat4 m);
	Mat4 operator-(Mat4 m);
	Vec4 operator*(Vec4 v);
//------
	Mat4 transposed();
	floatv det() {return det3(0, 1, 2, 0, 1, 2);};
};

#endif
