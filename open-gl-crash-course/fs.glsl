#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

const float offset = 1.0 / 300.0;

struct Material {
	sampler2D diffuse_texture1;
};

uniform Material material;

void main() {
	FragColor = texture(material.diffuse_texture1, TexCoords);
}