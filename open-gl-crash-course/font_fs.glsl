#version 330 core

out vec4 color;

in vec2 TexCoords;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
	color = vec4(textColor, texture(text, TexCoords).r);
}