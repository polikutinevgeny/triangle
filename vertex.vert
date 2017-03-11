#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 transform;

out vec4 ourColor;

void main() {
    gl_Position = model * vec4(position, 1.0);
    ourColor = vec4(color, 1.0);
}