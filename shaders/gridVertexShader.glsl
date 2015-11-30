#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 norm;
uniform mat4 trans;
uniform mat4 model;

out vec3 fragPos;
out vec3 normal;

void main() {
	normal = normalize(norm);
	fragPos = vec3(model*vec4(vertexPosition, 1.0));
    gl_Position = trans*vec4(vertexPosition, 1.0);
}