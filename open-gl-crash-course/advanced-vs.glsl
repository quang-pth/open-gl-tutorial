#version 330 core
// Vertex local position on the cubemap
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	// clip-space output position
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
