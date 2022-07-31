#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D diffuse_texture1;

void main() {
	FragColor = texture(diffuse_texture1, TexCoords);
}