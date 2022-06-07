#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out VS_INFO {
	vec2 TexCoords;
} vs_info;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vs_info.TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}