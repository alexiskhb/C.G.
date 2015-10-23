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

Vec4 Vec4::operator*(Vec4 v) {
	Vec4 result = Vec4();
	for(int i = 0; i < this->width*this->height; i++)
		result[i] = (*this)[i] * v[i];
	return result;
}

Vec4 Vec4::normalized() {
	Vec4 result = Vec4();
	float normDivider = sqrt(degreedElSum(2));
	if (!isValid() || normDivider < fabs(epsilon)) {
		result.valid = isValid() ? VTDIVBYZERO : VTERR;
		return result;
	}
	for(int i = 0; i < this->width*this->height; i++)
		result[i] = (*this)[i] / normDivider;
	return result;
}

void Vec4::normalize() {
	float normDivider = sqrt(degreedElSum(2));
	if (!isValid() || normDivider < fabs(epsilon)) {
		this->valid = isValid() ? VTDIVBYZERO : VTERR;
		return;
	}
	for(int i = 0; i < this->width*this->height; i++)
		(*this)[i] = (*this)[i] / normDivider;
}

Vec4 Vec4::operator+(float value) {
	Vec4 result = Vec4();
	operatorAdd(&result, value);
	return result;
}

Vec4 Vec4::operator*(float value) {
	Vec4 result = Vec4();
	operatorMult(&result, value);
	return result;
}

Vec4::~Vec4() {
	//delete [] this->data;
}
