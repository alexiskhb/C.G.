#ifndef CAMERAM_HPP_
#define CAMERAM_HPP_

#include "mat4.hpp"
#include "vec4.hpp"
#include <cmath>
#include <iostream>

class Camera {
public:
	Vec4 position;
	Vec4 target;
	Vec4 head;
	Camera();
	Camera(Vec4 p, Vec4 l, Vec4 u) : position(p.normalized()), target(l.normalized()), head(u.normalized()) {};
	Vec4 dir() {
		return Vec4(4, - target[0] + position[0], - target[1] + position[1], - target[2] + position[2]).normalized();
	};
	void MoveForward(const float speed);
	void MoveSideway(const float speed);
	void Rotate(const Vec4 &axis, const float angle);
	void lookAt(Vec4 target);
	Mat4 GetView();
	Mat4 projectionMatrix(floatv fov, floatv aspect, floatv near, floatv far);
	Mat4 perspective(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far);
	Mat4 ortho(floatv left, floatv right, floatv top, floatv bottom, floatv near, floatv far);
	friend std::ostream& operator<<(std::ostream &out, Camera c) {
		out << c.position[0] << "x " << c.position[1] << "y " << c.position[2] << "z pos| ";
		out << c.target[0]   << "x " << c.target[1]   << "y " << c.target[2]   << "z targ| ";
		out << c.head[0]     << "x " << c.head[1]     << "y " << c.head[2]     << "z head|\n";
		return out;
	}
};

#endif
