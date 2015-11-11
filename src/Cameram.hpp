#ifndef CAMERAM_HPP_
#define CAMERAM_HPP_

#include "mat4.hpp"
#include "vec4.hpp"

class Camera {
public:
	Vec4 position;
	Vec4 target;
	Vec4 up;
	Camera();
	Camera(const Vec4 &p, const Vec4 &l, const Vec4 &u) : position(p), target(l), up(u) {};
	void lookAt(const Vec4 &p, const Vec4 &l, const Vec4 &u);
};

#endif
