#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

uniform mat4 projection;

void main() {
	gl_Position = vec4(vertex.xy, 0.0, 1.0);
	vec2 Texture = vertex.zw;
	
	if (chaos) {
		float strength = 0.3;
		vec2 pos = vec2(Texture.x + sin(time) * strength, Texture.y + cos(time) * strength);
		TexCoords = pos;
	}
	else if (confuse) {
		TexCoords = vec2(1.0 - Texture.x, 1.0 - Texture.y);
	}
	else
	{
		TexCoords = Texture;	
	}

	if (shake) {
		float strength = 0.01;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;
	}
}