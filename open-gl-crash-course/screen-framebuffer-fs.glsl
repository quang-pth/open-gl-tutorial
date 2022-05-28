#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

void main() {
	FragColor = texture(screenTexture, TexCoords);
	float grayScale = 0.2126 * FragColor.r + 0.7512 * FragColor.g + 0.0722 * FragColor.b;
	FragColor = vec4(grayScale, grayScale, grayScale, 1.0);
}