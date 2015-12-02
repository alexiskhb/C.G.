#version 330 core

out vec4 color;

uniform vec3 incolor;
uniform float intense;

void main(void)
{
	color = intense*vec4(incolor, 1.0)/20;
}