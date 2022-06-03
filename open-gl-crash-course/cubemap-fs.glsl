#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skyboxTexture;

void main()
{
	vec3 viewDir = normalize(Position - cameraPos);
	vec3 reflectDir = reflect(viewDir, normalize(Normal));

	FragColor = vec4(texture(skyboxTexture, reflectDir).rgb, 1.0);
}