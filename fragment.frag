#version 450

in vec4 ourColor;
out vec4 color;

uniform float time;

void main() {
    color = ourColor;
}