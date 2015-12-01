#ifndef VEC4_HPP_
#define VEC4_HPP_

#include "mat.hpp"
#include "mat4.hpp"
#include <utility>
#include <algorithm>

class Vec4 : public Mat {
public:
	Vec4() : Mat(4, 1) {};
	template<typename Sizet, typename... Args> Vec4(Sizet asize, Args ... args) : Mat(asize, 1) {
		const int size = sizeof...(args);
		double a[size] = {args...};
		for(int i = 0; i < std::min(size, asize); i++)
			data[i] = static_cast<floatv>(a[i]);
		for(int i = std::min(size, asize); i < std::max(size, asize); i++)
			data[i] = 0.;
	}
	Vec4(const Vec4& orig);
	Vec4(ValidityT validity) : Mat(validity) {};
	virtual ~Vec4();
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
	static floatv dot  (Vec4 a, Vec4 b);
	inline floatv length()  {return sqrt(degreedElSum(2));};
	void   normalize();
	Vec4   normalized();
	Vec4   transposed();
	Vec4   cross3(Vec4 v) {return cross3(*this, v);};
	floatv dot  (Vec4 v) {return   dot(*this, v);};
};

#endif

