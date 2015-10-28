//vec3, v3c4
//int length(src, result)
//int normalize(srcvec, newvec) с кодом ошибки
//int normalizeinplace(src)
//int add(src1, src2, new)
//int sub(src1, src2, new)
//int nummult(src, num, new)
//int multpocomponentno()
//vec3 CreateVector(x, y, z)
//int Dot(src1, src2, new) <-- scalar mult
//int Cross(src1, scr2, new) <-- vector mult

#include "vec4.hpp"

Vec4::Vec4(const Vec4& orig) : Mat(orig.height, orig.width) {
	for(int i = 0; i < orig.height*orig.width; i++)
		this->data[i] = orig.data[i];
	this->valid = orig.valid;
}

Vec4 Vec4::bycomp(Vec4 a, Vec4 b) {
	Vec4 result = Vec4();
	for(int i = 0; i < a.width*b.height; i++)
		result[i] = a[i] * b[i];
	return result;
}

Mat4 Vec4::operator*(Vec4 v) {
	Mat4 result = Mat4();
	operatorMultMatr(result, v);
	return result;
}

Vec4 Vec4::normalized() {
	Vec4 result = Vec4();
	floatv normDivider = sqrt(degreedElSum(2));
	if (!isValid() || normDivider < fabs(epsilon)) {
		result.valid = isValid() ? VTDIVBYZERO : VTERR;
		return result;
	}
	for(int i = 0; i < this->width*this->height; i++)
		result[i] = (*this)[i] / normDivider;
	return result;
}

void Vec4::normalize() {
	floatv normDivider = sqrt(degreedElSum(2));
	if (!isValid() || normDivider < fabs(epsilon)) {
		this->valid = isValid() ? VTDIVBYZERO : VTERR;
		return;
	}
	for(int i = 0; i < this->width*this->height; i++)
		(*this)[i] = (*this)[i] / normDivider;
}

Vec4 Vec4::operator+(floatv value) {
	Vec4 result = Vec4();
	operatorAdd(result, value);
	return result;
}

Vec4 Vec4::operator*(floatv value) {
	Vec4 result = Vec4();
	operatorMult(result, value);
	return result;
}

Vec4 Vec4::operator+(Vec4 v) {
	Vec4 result = Vec4();
	operatorAdd(result, v, 1);
	return result;
}

Vec4 Vec4::operator-(Vec4 v) {
	Vec4 result = Vec4();
	operatorAdd(result, v, -1);
	return result;
}

Vec4 Vec4::cross(Vec4 a, Vec4 b) {
	Vec4 result = Vec4(
			a.Y()*b.Z() - a.Z()*b.Y(),
			a.Z()*b.X() - a.X()*b.Z(),
			a.X()*b.Y() - a.Y()*b.X());
	if (!a.isValid() || !b.isValid())
		result.valid = VTERR;
	return result;
}

floatv Vec4::dot(Vec4 a, Vec4 b) {
	floatv result = 0.0;
	result = a.X()*b.X() + a.Y()*b.Y() + a.Z()*b.Z();
	return result;
}

Vec4::~Vec4() {
	//delete [] this->data;
}
