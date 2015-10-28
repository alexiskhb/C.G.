//mat4 createmat()
//mat4 createidentity()
//mat4 transpose()
//add (mat1, mat2)
//sub(mat1, mat2)
//mult(mat, num)
//mult(mat, mat)
//mult(mat, vec)

//int determinant()
//int inverse() (LU, SVD)-methods

#include "mat4.hpp"

using namespace std;

Mat4::Mat4(const Mat4 &orig) : Mat(orig.height, orig.width) {
	for(int i = 0; i < orig.height*orig.width; i++)
		this->data[i] = orig.data[i];
	this->valid = orig.valid;
}

Mat4 Mat4::createIdent() {
	Mat4 result = Mat4();
	makeIdent(result);
	return result;
}

Mat4 Mat4::operator+(floatv value) {
	Mat4 result = Mat4();
	operatorAdd(result, value);
	return result;
}

Mat4 Mat4::operator*(floatv value) {
	Mat4 result = Mat4();
	operatorMult(result, value);
	return result;
}

Mat4 Mat4::operator*(Mat4 m) {
	Mat4 result = Mat4();
	operatorMultMatr(result, m);
	return result;
}

Mat4 Mat4::operator+(Mat4 m) {
	Mat4 result = Mat4();
	operatorAdd(result, m, 1);
	return result;
}

Mat4 Mat4::operator-(Mat4 m) {
	Mat4 result = Mat4();
	operatorAdd(result, m, -1);
	return result;
}

Vec4 Mat4::operator*(Vec4 v) {
	Vec4 result = Vec4();
	operatorMultMatr(result, v);
	return result;
}

Mat4 Mat4::transposed() {
	Mat4 result = Mat4(*this);
	result.transpose();
	return result;
}

Mat4::~Mat4() {
	//delete [] this->data;
}

