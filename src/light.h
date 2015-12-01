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
	Light(const Camera &cam, Vec4 _color, float _intense, float _spotAngle) : type(SPOT), color(_color), intense(_intense), spotAngle(cos(_spotAngle)){
		Vec4 t = cam.position;
		t -= cam.direction;
		state = Camera(cam.position, t, Vec4(4, 0., 1., 0.));
	}
	Light(const Camera &cam, Vec4 _color, float _intense, light_t _type) : type(_type), color(_color), intense(_intense), spotAngle(-1){
		Vec4 t = cam.position;
		t += cam.direction;
		state = Camera(cam.position, t, Vec4(4, 0., 1., 0.));
	}
	void Uniform(int i, Program prog) {
		std::stringstream s; s << i;
		Vec4 pos = state.position;
		Vec4 dir = state.direction;
		if (disabled)
			glUniform1f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].disabled").c_str()), disabled);
		else {
			glUniform3f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].color").c_str()), color[0], color[1], color[2]);
			glUniform1i(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].type").c_str()), type);
			glUniform1f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].intense").c_str()), intense);
			if (type != DIR)
				glUniform3f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].pos").c_str()), pos[0], pos[1], pos[2]);
			if (type != POINT)
				glUniform3f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].dir").c_str()), dir[0], dir[1], dir[2]);
			if (type == SPOT)
				glUniform1f(glGetUniformLocation(prog.handler, ("lights[" + s.str() + "].spotAngle").c_str()), spotAngle);
		}
	}
	light_t type;
	Camera state;
	Vec4   color;
	int disabled = 0;
	float  intense;
	float spotAngle;
	Light();
};

#endif /* LIGHT_H_ */
