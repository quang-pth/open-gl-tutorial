#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	vec4 worldSpacePos = model * vec4(aPos, 1.0);
	WorldPos = worldSpacePos.xyz;

	TexCoords = aTexCoords;
	
	mat3 normalMaxtrix = transpose(inverse(mat3(model)));
	Normal = normalMaxtrix * aNormal;

	gl_Position = projection * view * worldSpacePos;
}

