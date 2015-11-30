#ifndef LIGHT_H_
#define LIGHT_H_

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "Cameram.hpp"
#include <sstream>
#include <string>
#include "glprog.hpp"


typedef enum {
	NONE, DIR = 1, SPOT = 2, POINT = 3
} light_t;

class Light {
public:
	Light(Vec4 _pos, Vec4 _dir, Vec4 _color, float _intense, float _spotAngle) : color(_color), intense(_intense), spotAngle(_spotAngle){
		type = SPOT;
		state = Camera(_pos, _pos+_dir, Vec4(4, 0., 1.));
	}
	Light(light_t _type, Vec4 _state, Vec4 _color, float _intense) : type(_type), color(_color), intense(_intense), spotAngle(-1){
		if (_type == POINT) {
			state = Camera(_state, Vec4(4, 100., 0., 100.), Vec4(4, 0., 1., 0.));
		} else {
			state = Camera(Vec4(4, 1.), Vec4(4), Vec4(4, 0., 1.));
			state.direction = _state;
		}
	}
	void Uniform(int i, Program prog) {
		std::stringstream s; s << i;
		Vec4 pos = state.position;
		Vec4 dir = state.direction;
		glUniform3f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].color").c_str()), color[0], color[1], color[2]);
		glUniform3f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].pos").c_str()), pos[0], pos[1], pos[2]);
		glUniform3f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].dir").c_str()), dir[0], dir[1], dir[2]);
		glUniform1i(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].type").c_str()), type);
		glUniform1f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].spotAngle").c_str()), spotAngle);
	}
	light_t type;
	Camera state;
	Vec4   color;
	float  intense;
	float spotAngle;
	Light();
};

#endif /* LIGHT_H_ */
