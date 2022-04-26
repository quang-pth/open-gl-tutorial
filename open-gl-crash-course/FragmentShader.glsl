#version 330 core

in vec3 FragPos; // vetex position in world coordinate
in vec3 Normal; // perpendicular with the vertex

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	// constant ambient factor
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// Calc Diffuse component
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); // get direction light ray from light source to the fragment
	float diffuseFactor = max(dot(normal, lightDir), 0);
	vec3 diffuse = diffuseFactor * lightColor; // light is affected by the diffuse component

	// Calc Specular component
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal); // negate the light direction because we need a light direction from light source to the fragment
	float specularFactor = pow(max(dot(viewDir, reflectDir), 0), 32); // 32 is the shininess value
	vec3 specular = specularStrength * specularFactor * lightColor;

	// Combine ambient and diffuse and specular component to create object color
	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result, 1.0);
}