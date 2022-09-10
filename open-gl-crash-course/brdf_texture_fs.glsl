#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D brdfTexture;

void main() {
	FragColor = vec4(texture(brdfTexture, TexCoords).rgb, 1.0);
}