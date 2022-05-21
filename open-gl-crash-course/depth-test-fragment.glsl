#version 330

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Texture;

float near = 0.1;
float far = 100.0;

float LinearizeDepth(float depth) {
	// convert z to normalize device coords
	float ndc = depth * 2.0 - 1.0;
	// return range between near far
	return (2 * near * far) / (near + far - ndc * (far - near));
}

void main() {
	FragColor = texture(Texture, TexCoords);
//	float depth = LinearizeDepth(gl_FragCoord.z); // far;  convert to range [0, 1]
//	FragColor = vec4(vec3(depth), 1.0);
}