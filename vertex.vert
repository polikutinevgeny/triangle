#version 450

layout (location = 0) in vec3 position;

uniform vec3 positionShift;

void main() {
    gl_Position = vec4(position + positionShift, 1.0);
}