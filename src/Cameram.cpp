#include "Cameram.hpp"

Camera::Camera() {

}

void Camera::MoveForward(const float speed) {
	Vec4 direction = forward;//Vec4(4, 0., 0., -1.);//dir();
	this->position += direction * speed;
	this->target   += direction * speed;
}

void Camera::MoveSideway(const float speed) {
	Vec4 direction = side;//Vec4(4, -1., 0., 0.);//(camUp().cross3(dir())).normalized();
	this->position += direction * speed;
	this->target   += direction * speed;
}

void Camera::MoveUp(const float speed) {
	Vec4 direction = worldUp();
	this->position += direction * speed;
	this->target   += direction * speed;
}

void Camera::Rotate(const Vec4 &axis, const float angle) {
	float cosx = cos(angle/180.*M_PI);
	float sinx = sin(angle/180.*M_PI);
	floatv x = axis.data[0], y = axis.data[1], z = axis.data[2];

	Mat4 rotation = Mat4(
		Vec4(4, cosx + x*x*(1 - cosx), x*y*(1 - cosx) - z*sinx, x*z*(1 - cosx) + y*sinx),
		Vec4(4, x*y*(1 - cosx) + z*sinx, cosx + y*y*(1 - cosx), y*z*(1 - cosx) - x*sinx),
		Vec4(4, z*x*(1 - cosx) - y*sinx, y*z*(1 - cosx) + x*sinx, cosx + z*z*(1 - cosx)),
		Vec4(4, 0., 0., 0., 0.));

	this->side = (rotation*this->side).normalized();
	this->forward = (rotation*this->forward).normalized();
	this->up = (rotation*this->up).normalized();
}

Mat4 Camera::GetView() {
	Vec4
			c = forward,
			a = side,
			b = up;
	if (true)
	return
			Mat4(a, b, c, Vec4(4, 0., 0., 0., 1.))*
			Mat4(
				Vec4(4, 1., 0., 0., -position[0]),
				Vec4(4, 0., 1., 0., -position[1]),
				Vec4(4, 0., 0., 1., -position[2]),
				Vec4(4, 0., 0., 0., 1.));
	return Mat4(
		Vec4(4, a[0], b[0], c[0], -position[0]),
		Vec4(4, a[1], b[1], c[1], -position[1]),
		Vec4(4, a[2], b[2], c[2], -position[2]),
		Vec4(4, 0.  , 0., 0., 1.));
}


Mat4 Camera::projectionMatrix(floatv fov, floatv aspect, floatv near, floatv far) {
	fov = fov*M_PI/180.;
	floatv
		top = tan(fov/2.)*near,
		bottom = -top,
		right = top*aspect,
		left = -right;
	return projectionMatrix(left, right, top, bottom, near, far);
}

Mat4 Camera::ortho(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far) {
	return Mat4(
		Vec4(4, 2./(right - left), 0., 0., (right + left)/(left - right)),
		Vec4(4, 0., 2./(top - bottom), 0., (top + bottom)/(bottom - top)),
		Vec4(4, 0., 0., 2./(near - far), (far + near)/(near - far)),
		Vec4(4, 0., 0., 0., 1.));
}

Mat4 Camera::projectionMatrix(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far) {
	return Mat4(
		Vec4(4, 2.*near/(right - left), 0., (right + left)/(right - left)),
		Vec4(4, 0., 2.*near/(top - bottom), (top + bottom)/(top - bottom)),
		Vec4(4, 0., 0., (near + far) / (near - far), 2.*far*near/(near - far)),
		Vec4(4, 0., 0., -1., 0.));
}
