#version 330

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D Texture;

void main() {
	vec4 fragmentColor = texture(Texture, TexCoords);

	// Discard transparent fragment
	if (fragmentColor.a < 0.1) {
		discard;
	}
	
	FragColor = fragmentColor;
}