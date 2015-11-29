#ifndef LIGHT_H_
#define LIGHT_H_

#include "Cameram.hpp"

class Light {
public:
	Camera state;
	Vec4   color;
	float  intense;
	Light();
	virtual ~Light();
};

#endif /* LIGHT_H_ */
