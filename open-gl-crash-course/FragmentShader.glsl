#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec4 vertexPosition;

void main() {
	// FragColor = vec4(ourColor, 1.0);
	FragColor = vertexPosition;
}