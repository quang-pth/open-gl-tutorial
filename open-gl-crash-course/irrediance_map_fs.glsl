#version 330 core

out vec4 FragColor;

in vec3 localPosition;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main() {
	vec3 normal = normalize(localPosition);
	vec3 irrediance = vec3(0.0);

	// Calculate irrediance map by convolution p414
	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);

	float sampleData = 0.025;
	float nrSamples = 0;
	for (float phi = 0; phi < 2 * PI; phi += sampleData) {
		for (float theta = 0; theta < PI / 2; theta += sampleData) {
			// Convert Spherical coords to Cartesian coords in tangent space
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// Tangent space to World space
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

			irrediance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
			nrSamples += 1;
		} 
	}

	irrediance = irrediance * PI * (1 / nrSamples);

	FragColor = vec4(irrediance, 1.0);
}