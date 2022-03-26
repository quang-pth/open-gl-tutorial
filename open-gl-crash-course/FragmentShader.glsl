#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D ourTexture; // sampler2D: texture objects data-type

void main() {
	FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0);
}