//vec3, v3c4
//int length(src, result)
//int normalize(srcvec, newvec) с кодом ошибки
//int normalizeinplace(src)
//int add(src1, src2, new)
//int sub(src1, src2, new)
//int nummult(src, num, new)
//int multpocomponentno()
//int Dot(src1, src2, new) <-- scalar mult
//int Cross(src1, scr2, new) <-- vector mult
//vec3 CreateVector(x, y, z)

#include "vec4.hpp"

Vec4::Vec4() : Mat(4, 1) {

}





Vec4::~Vec4() {
	delete [] this->data;
}

float Vec4::Length() {
	return sqrt(DegreedElSum(2));
}

float Vec4::x() {
	return this->data[0];
}

float Vec4::x(float value) {
	return this->data[0] = value;
}

float Vec4::y() {
	return this->data[1];
}

float Vec4::y(float value) {
	return this->data[1] = value;
}

float Vec4::z() {
	return this->data[2];
}

float Vec4::z(float value) {
	return this->data[2] = value;
}

float Vec4::w() {
	return this->data[3];
}

float Vec4::w(float value) {
	return this->data[3] = value;
}
