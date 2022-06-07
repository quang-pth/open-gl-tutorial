#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out VS_INFO {
	vec3 color;
} vs_info;

void main() {
	vs_info.color = aColor;
	gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}