#version 330 core

in vec3 localPosition;
in vec3 Normal;

out vec4 FragColor;

uniform samplerCube environmentMap;

void main() {
	vec3 envColor = textureLod(environmentMap, localPosition, 0.0).rgb;
	// HDR capture
	envColor = envColor / (envColor + vec3(1.0));
	// Gamma correction
	envColor = pow(envColor, vec3(1.0 / 2.2));
	
	FragColor = vec4(envColor, 1.0);
}