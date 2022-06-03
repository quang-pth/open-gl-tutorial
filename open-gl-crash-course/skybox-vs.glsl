#version 330 core
// Vertex local position on the cubemap
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	TexCoords = aPos;
	// Not translate the cubemap
	vec4 pos = projection * view * vec4(aPos, 1.0);
	// set depth value to 1.0 (w/w = 1.0)
	gl_Position = pos.xyww;
}
