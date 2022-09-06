#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 localPosition;

uniform mat4 projection;
uniform mat4 view;

void main() {
	localPosition = aPos;
	// Remove the translation
	mat4 rotView = mat4(mat3(view));
	vec4 clipPosition = projection * rotView * vec4(localPosition, 1.0);

	gl_Position = clipPosition.xyww;
}