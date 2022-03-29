#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

// sampler2D: texture objects data-type
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float ratio;

void main() {
	FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), ratio);
}