#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 normal;
uniform mat4 trans;

void main() {
    gl_Position = trans*vec4(vertexPosition, 1.0);
}