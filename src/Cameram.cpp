#include "Cameram.hpp"

Camera::Camera() {

}

void Camera::lookAt(Vec4 a_target) {
	target = a_target.normalized();
	head   = (dir().cross3(Vec4(4, position[1] - target[1], target[0] - position[0], position[2]))).normalized();
}

void Camera::MoveForward(const float speed) {
	Vec4 direction = dir();
	this->position[0] += speed * direction[0];
	this->position[1] += speed * direction[1];
//	position[2] += speed * direction[2];

	this->target[0] += speed * direction[0];
	this->target[1] += speed * direction[1];
//	target[2] += speed * direction[2];
}

void Camera::MoveSideway(const float speed) {
	Vec4 direction = dir().cross3(head);
	this->position[0] += speed * direction[0];
	this->position[1] += speed * direction[1];
//	position[2] += speed * direction[2];

	this->target[0] += speed * direction[0];
	this->target[1] += speed * direction[1];
//	target[2] += speed * direction[2];
}

void Camera::Rotate(const Vec4 &axis, const float angle) {
	float cosx = cos(angle/180.*M_PI);
	float sinx = sin(angle/180.*M_PI);
	Vec4 newTarget;
	Vec4 direction = dir().normalized();
	floatv x = axis.data[0], y = axis.data[1], z = axis.data[2];

	Mat4 rotation = Mat4(
			Vec4(4, cosx + x*x*(1 - cosx), x*y*(1 - cosx) - z*sinx, x*z*(1 - cosx) + y*sinx),
			Vec4(4, x*y*(1 - cosx) + z*sinx, cosx + y*y*(1 - cosx), y*z*(1 - cosx) - x*sinx),
			Vec4(4, z*x*(1 - cosx) - y*sinx, y*z*(1 - cosx) + x*sinx, cosx + z*z*(1 - cosx)),
			Vec4(4));

	newTarget[0]  = ((1 - cosx)*x*x + cosx)   * direction[0];
	newTarget[0] += ((1 - cosx)*x*y - z*sinx) * direction[1];
	newTarget[0] += ((1 - cosx)*x*z + y*sinx) * direction[2];

	newTarget[1]  = ((1 - cosx)*x*y + z*sinx) * direction[0];
	newTarget[1] += ((1 - cosx)*y*y + cosx)   * direction[1];
	newTarget[1] += ((1 - cosx)*y*z - x*sinx) * direction[2];

	newTarget[2]  = ((1 - cosx)*x*z - y*sinx) * direction[0];
	newTarget[2] += ((1 - cosx)*y*z + x*sinx) * direction[1];
	newTarget[2] += ((1 - cosx)*z*z + cosx)   * direction[2];

	target = (position + newTarget).normalized();
}

Mat4 Camera::GetView() {
	Mat4 r;
	Vec4 T = dir().normalized();
	Vec4 R = ((head).cross3(target)).normalized();
	Vec4 U = head;

	r(0, 0) = R[0]; r(0, 1) = R[1]; r(0, 2) = R[2]; r(0, 3) = position[0];
	r(1, 0) = U[0]; r(1, 1) = U[1]; r(1, 2) = U[2]; r(1, 3) = position[1];
	r(2, 0) = T[0]; r(2, 1) = T[1]; r(2, 2) = T[2]; r(2, 3) = position[2];
	r(3, 0) = 0.0 ; r(3, 1) = 0.0 ; r(3, 2) = 0.0 ; r(3, 3) = 1.;

	return r;
}


Mat4 Camera::projectionMatrix(floatv fov, floatv aspect, floatv near, floatv far) {
	fov = fov*M_PI/180.;
	return Mat4(
			Vec4(4, 1./aspect/tanf(fov/2.)),
			Vec4(4, 0., 1./tanf(fov/2.)),
			Vec4(4, 0., 0., (near + far) / (near - far), (2.*far*near) / (near - far)),
			Vec4(4, 0., 0., -1., 0.));
}

Mat4 Camera::ortho(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far) {
	return Mat4(
		Vec4(4, 2./(right - left), 0., 0., (right + left)/(left - right)),
		Vec4(4, 0., 2./(top - bottom), 0., (top + bottom)/(bottom - top)),
		Vec4(4, 0., 0., 2./(near - far), (far + near)/(near - far)),
		Vec4(4, 0., 0., 0., 1.));
}

Mat4 perspective(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far) {
	return Mat4(
		Vec4(4, 2.*near/(right - left), 0., (right + left)/(right - left)),
		Vec4(4, 0., 2.*near/(top - bottom), (top + bottom)/(top - bottom)),
		Vec4(4, 0., 0., (near + far) / (near - far), 2.*far*near/(near - far)),
		Vec4(4, 0., 0., -1., 0.));
}
