#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() {
	float depthMapValue = texture(depthMap, TexCoords).r;
	FragColor = vec4(vec3(LinearizeDepth(depthMapValue) / far_plane), 1.0);
}