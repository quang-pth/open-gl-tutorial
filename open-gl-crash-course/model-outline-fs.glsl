#version 330 core

out vec4 FragColor;

uniform float time;

void main() {
	FragColor = vec4(1.0 - vec3(sin(time), 1.0, cos(time)), 1.0);
}