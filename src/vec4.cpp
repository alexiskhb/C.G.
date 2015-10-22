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

Vec4 Vec4::operator*(Vec4 v) {
	Vec4 result = Vec4();
	for(int i = 0; i < this->width*this->height; i++)
		result[i] = this->data[i] * v[i];
	return result;
}

Vec4::~Vec4() {
	delete [] this->data;
}
