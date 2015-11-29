#version 330 core

#define DIR 0
#define SPOT 1
#define POINT 2

float ambientStrength = 0.17f;

uniform int lightamt;

out vec4 color;

in vec3 modelPos;
in vec3 normal;

struct Light {
	int type;
	vec3 dir;
	vec3 color;
	vec3 pos;
    float A, B, C;
    float spotAngle;
};

uniform Light lights[100];

void main(void)
{
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    //vec3 cubeColor = vec3(1.0, 0.32, 1.0);
    //vec3 cubeColor = vec3(0.978, 0.488, 0.0);
    vec3 cubeColor = vec3(1.0, 1.0, 1.0);
	color = vec4(ambient*cubeColor, 1.0);
}