#ifndef CAMERAM_HPP_
#define CAMERAM_HPP_

#include "mat4.hpp"
#include "vec4.hpp"
#include <cmath>
#include <iostream>
#include <iomanip>

class Camera {
public:
	Vec4 position;
	Vec4 target;
	Vec4 head;
	Vec4 forward;
	Vec4 side;
	Camera();
	Camera(Vec4 p, Vec4 l, Vec4 u) : position(p), target(l), head(u) {};
	Vec4 dir() {
		return (position - target).normalized();
	};
	Vec4 worldUp() {
		return Vec4(4, 0., 1., 0.);
	};
	Vec4 rightHand() {
		return (head.cross3(dir())).normalized();
	}
	Vec4 camUp() {
		return (head/*dir().cross3(rightHand())*/).normalized();
	}
	void MoveForward(const float speed);
	void MoveSideway(const float speed);
	void MoveUp(const float speed);
	void Rotate(const Vec4 &axis, const float angle);
	Mat4 GetView();
	Mat4 projectionMatrix(floatv fov, floatv aspect, floatv near, floatv far);
	Mat4 projectionMatrix(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far);
	Mat4 ortho(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far);
	Mat4 ortho2(floatv fov, floatv aspect, floatv near, floatv far) {
		floatv top = tan(fov/2.)*near, bottom = -top, right = top*aspect, left = -right;
		return ortho(left, right, top, bottom, near, far);
	}
	friend std::ostream& operator<<(std::ostream &out, Camera c) {
		std::fixed(out);
		out << "P: " << std::setprecision(3) << c.position[0] << "x " << c.position[1] << "y " << c.position[2] << "z " << c.position[3] << "w";
		out << "\nT: " << std::setprecision(3) << c.target[0]   << "x " << c.target[1]   << "y " << c.target[2] << "z " << c.target[3] << "w";
		out << "\nD: " << std::setprecision(3) << c.dir()[0]   << "x " << c.dir()[1]   << "y " << c.dir()[2] << "z " << c.dir()[3] << "w\n\n";
		return out;
	}
};

#endif
