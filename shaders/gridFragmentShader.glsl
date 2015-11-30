#version 330 core
out vec4 color;
void main(void)
{
	vec3 color3 = vec3(1, 1, 1)*0.0;
	color = vec4(color3, 1.0);
	//color = vec4(0.8);
}
//out vec3 color;