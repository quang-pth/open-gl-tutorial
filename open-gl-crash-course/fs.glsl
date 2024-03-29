#version 330 core

out vec4 FragColor;

uniform float time;

void main() {
	FragColor = vec4(sin(time), 1.0, cos(time), 1.0);
}