#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skyboxTexture;

void main()
{
	vec3 viewDir = normalize(Position - cameraPos);
	// ratio: air refraction idx / glass refraction idx
    float ratio = 1.0 / 1.52;
	vec3 reflectDir = refract(viewDir, normalize(Normal), ratio);

	FragColor = vec4(texture(skyboxTexture, reflectDir).rgb, 1.0);
}