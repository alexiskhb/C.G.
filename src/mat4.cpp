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

Mat4::Mat4(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3, const Vec4 &v4) : Mat(4, 4) {
	Vec4 a[4] = {v1, v2, v3, v4};
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			this->data[i*4 + j] = a[i][j];
}

Mat4::Mat4(const Vec4 &v1, const Vec4 &v2, const Vec4 &v3) : Mat4(v1, v2, v3, Vec4(3)) {};

Mat4 Mat4::ident() {
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

Mat4 Mat4::operator*(const Mat4 &m) {
	Mat4 result = Mat4();
	operatorMultMatr(result, m);
	return result;
}

Mat4 Mat4::multMatr3(Mat4 m) {
	Mat4 result;
	this->height = this->width = m.height = m.width = 3;
	operatorMultMatr(result, m);
	this->height = this->width = m.height = m.width = 4;
	return result;
}

Mat4 Mat4::operator+(const Mat4 &m) {
	Mat4 result = Mat4();
	operatorAdd(result, m, 1);
	return result;
}

Mat4 Mat4::operator-(const Mat4 &m) {
	Mat4 result = Mat4();
	operatorAdd(result, m, -1);
	return result;
}

Vec4 Mat4::operator*(const Vec4 &v) {
	Vec4 result = Vec4(4, 5.0, 6.0, 7.0, 8.0);
	operatorMultMatr(result, v);
	return result;
}

Mat4 Mat4::transposed() {
	Mat4 result = Mat4(*this);
	result.transpose();
	return result;
}

floatv Mat4::det4() {
	floatv result = 0.;
	Mat4 a = (*this);
	result += a(0, 0) * det3(1, 2, 3, 1, 2, 3);
	result -= a(0, 1) * det3(1, 2, 3, 0, 2, 3);
	result += a(0, 2) * det3(1, 2, 3, 0, 1, 3);
	result -= a(0, 3) * det3(1, 2, 3, 0, 1, 2);
	return result;
}

Mat4 Mat4::inversed3() {
	Mat4 res;
	floatv div = det3();
	if (fabs(div) < epsilon) {
		res.valid = VTSINGULAR;
		return res;
	}
	res(0, 0) =  det2(1, 2, 1, 2)/div;
	res(0, 1) = -det2(0, 2, 1, 2)/div;
	res(0, 2) =  det2(0, 1, 1, 2)/div;
	res(1, 0) = -det2(1, 2, 0, 2)/div;
	res(1, 1) =  det2(0, 2, 0, 2)/div;
	res(1, 2) = -det2(0, 1, 0, 2)/div;
	res(2, 0) =  det2(1, 2, 0, 1)/div;
	res(2, 1) = -det2(0, 2, 0, 1)/div;
	res(2, 2) =  det2(0, 1, 0, 1)/div;
	return res;
}

Mat4 Mat4::inversed() {
	Mat4 res;
	floatv div = det4();
	if (fabs(div) < epsilon) {
		res.valid = VTSINGULAR;
		return res;
	}
	res(0, 0) =  det3(1, 2, 3, 1, 2, 3)/div;
	res(0, 1) = -det3(0, 2, 3, 1, 2, 3)/div;
	res(0, 2) =  det3(0, 1, 3, 1, 2, 3)/div;
	res(0, 3) = -det3(0, 1, 2, 1, 2, 3)/div;
	res(1, 0) = -det3(1, 2, 3, 0, 2, 3)/div;
	res(1, 1) =  det3(0, 2, 3, 0, 2, 3)/div;
	res(1, 2) = -det3(0, 1, 3, 0, 2, 3)/div;
	res(1, 3) =  det3(0, 1, 2, 0, 2, 3)/div;
	res(2, 0) =  det3(1, 2, 3, 0, 1, 3)/div;
	res(2, 1) = -det3(0, 2, 3, 0, 1, 3)/div;
	res(2, 2) =  det3(0, 1, 3, 0, 1, 3)/div;
	res(2, 3) = -det3(0, 1, 2, 0, 1, 3)/div;
	res(3, 0) = -det3(1, 2, 3, 0, 1, 2)/div;
    res(3, 1) =  det3(0, 2, 3, 0, 1, 2)/div;
	res(3, 2) = -det3(0, 1, 3, 0, 1, 2)/div;
	res(3, 3) =  det3(0, 1, 2, 0, 1, 2)/div;
	return res;
}

Mat4 Mat4::perspective(Vec4 v) {
	return (*this)*Mat4(
			Vec4(4, 1., 0., 0., v[0]),
			Vec4(4, 0., 1., 0., v[1]),
			Vec4(4, 0., 0., 1., v[2]),
			Vec4(4, 0., 0., 0., 1.));
}

Mat4 Mat4::translated(Vec4 v) {
	return (*this)*Mat4(
			Vec4(4, 1., 0., 0., v[0]),
			Vec4(4, 0., 1., 0., v[1]),
			Vec4(4, 0., 0., 1., v[2]),
			Vec4(4, 0., 0., 0., 1.));
}

Mat4 Mat4::scale(Vec4 v) {
	return (*this)*Mat4(
			Vec4(4, v[0], 0., 0., 0.),
			Vec4(4, 0., v[1], 0., 0.),
			Vec4(4, 0., 0., v[2], 0.),
			Vec4(4, 0., 0., 0.,   1.));
}

Mat4 Mat4::rotate(Vec4 v, floatv angle) {
	Mat4 general_rotation = Mat4::ident();
	angle = angle/180.*M_PI;
	if (fabs(v[0]) >= epsilon) {
		general_rotation = general_rotation * Mat4(
				Vec4(4, 1., 0., 0., 0.),
				Vec4(4, 0., cos(angle), -sin(angle), 0.),
				Vec4(4, 0., sin(angle),  cos(angle), 0.),
				Vec4(4, 0., 0., 0., 1.));
	}
	if (fabs(v[1]) >= epsilon) {
		general_rotation = general_rotation * Mat4(
				Vec4(4,  cos(angle), 0., sin(angle), 0.),
				Vec4(4, 0., 1., 0., 0.),
				Vec4(4, -sin(angle), 0., cos(angle), 0.),
				Vec4(4, 0., 0., 0., 1.));

	}
	if (fabs(v[2]) >= epsilon) {
		general_rotation = general_rotation * Mat4(
				Vec4(4, cos(angle), -sin(angle), 0., 0.),
				Vec4(4, sin(angle),  cos(angle), 0., 0.),
				Vec4(4, 0., 0., 1., 0.),
				Vec4(4, 0., 0., 0., 1.));

	}
	return general_rotation*(*this);
}


Mat4::~Mat4() {
	//delete [] this->data;
}

