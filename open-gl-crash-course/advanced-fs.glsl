#version 330 core 

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main() {
	if (gl_FrontFacing) {
		FragColor = texture(frontTexture, TexCoords);
	}
	else {
		FragColor = texture(backTexture, TexCoords);
	}
//	FragColor = texture(frontTexture, TexCoords);
}