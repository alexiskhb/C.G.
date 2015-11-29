#ifndef LIGHT_H_
#define LIGHT_H_

#include "Cameram.hpp"

typedef enum {
	DIR, SPOT, POINT
} light_t;

class Light {
public:
	Light(Vec4 _dir, Vec4 _color, float _intense) : color(_color), intense(_intense), spotAngle(-1){
		type = DIR;
		state = Camera(Vec4(4, 1.), Vec4(4), Vec4(4, 0., 1.));
		state.direction = _dir;
	}
	Light(Vec4 _pos, Vec4 _dir, Vec4 _color, float _intense, float _spotAngle) : color(_color), intense(_intense), spotAngle(_spotAngle){
		type = SPOT;
		state = Camera(_pos, _pos+_dir, Vec4(4, 0., 1.));
	}
	Light(Vec4 _pos, Vec4 _color, float _intense) : color(_color), intense(_intense), spotAngle(-1){
		type = POINT;
		state = Camera(_pos, Vec4(4), Vec4(4, 0., 1.));
	}
	light_t type;
	Camera state;
	Vec4   color;
	float  intense;
	float spotAngle;
	Light();
	virtual ~Light();
};

#endif /* LIGHT_H_ */
