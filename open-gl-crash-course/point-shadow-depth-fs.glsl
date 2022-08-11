#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main() {
	// distance between light and fragment postion
	float lightDistance = length(FragPos.xyz - lightPos);
	// map to range [0, 1]
	lightDistance = lightDistance / far_plane;
	// write this as modified depth
	gl_FragDepth = lightDistance;
}