#version 330 core
out vec4 color;
void main(void)
{
	float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
    vec3 cubeColor = vec3(0.9, 0.3, 0.9);
    //vec3 cubeColor = vec3(0.978, 0.488, 0.0);
    //vec3 cubeColor = vec3(1.0, 1.0, 1.0);
	color = vec4(ambient*cubeColor, 1.0);
}