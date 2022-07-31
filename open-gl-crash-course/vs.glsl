#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_INFO {
	vec3 Normal;
} vs_info;

layout (std140) uniform Matrices {
	mat4 view;
};

uniform mat4 model;

void main() {
	gl_Position = view * model * vec4(aPos, 1.0);
	// Convert normal vector to view-space coords (due to gs operates vertex on view-space coords)
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_info.Normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}