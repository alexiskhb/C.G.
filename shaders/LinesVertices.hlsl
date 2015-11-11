#version 330 core
layout(location = 0) in vec3 vertexPosition;
uniform mat4 trans;

void main() {
    gl_Position = trans*vec4(vertexPosition, 1.0);
}