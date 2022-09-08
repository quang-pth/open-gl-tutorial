#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 localPosition;

uniform mat4 projection;
uniform mat4 view;

void main() {
	localPosition = aPos;
	
	gl_Position = projection * view * vec4(localPosition, 1.0);
}