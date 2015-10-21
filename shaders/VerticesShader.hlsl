#version 330 core
layout(location = 0) in vec3 vertexPosition;
void main(void)
{
	//gl_Position = vec4(position, 1.0);
	gl_Position.xyz = vertexPosition;
	gl_Position.w = 1.0;
}
//#version 330 core
//in vec3 vertexPosition;