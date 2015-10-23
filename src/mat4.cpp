//mat4 createmat()
//mat4 createidentity()
//mat4 transpose()
//int determinant()
//int inverse() (LU, SVD)-methods
//add (mat1, mat2)
//sub(mat1, mat2)
//mult(mat, num)
//mult(mat, mat)
//mult(mat, vec)

#include "mat4.hpp"

Mat4::Mat4() : Mat(4, 4) {

}

Mat4::~Mat4() {
	//delete [] this->data;
}

