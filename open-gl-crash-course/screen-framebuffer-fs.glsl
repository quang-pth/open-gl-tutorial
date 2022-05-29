#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

void main() {
	vec2 offset[9] = vec2[] (
		vec2(-offset, offset), // top-left
		vec2( 0.0, offset), // top-center
		vec2( offset, offset), // top-right
		vec2(-offset, 0.0), // center-left
		vec2( 0.0, 0.0), // center-center
		vec2( offset, 0.0), // center-right
		vec2(-offset, -offset), // bottom-left
		vec2( 0.0, -offset), // bottom-center
		vec2( offset, -offset) // bottom-right
	);

	// kernel matrix
	float kernel[9] = float[] (
		1.0 / 16,  2.0 / 16,  1.0 / 16,
		2.0 / 16,  4.0 / 16,  2.0 / 16,
		1.0 / 16,  2.0 / 16,  1.0 / 16
	);
	
	vec3 sampleTex[9];
	// Offset fragment in surrounding directions
	for (int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offset[i]));
	}
	// Combine fragment color
	vec3 post_processing_effect = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		post_processing_effect += sampleTex[i] * kernel[i];
	}

	FragColor = vec4(post_processing_effect, 1.0);
}