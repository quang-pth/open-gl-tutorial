#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// geometry shader recives input as a set of vertices
in VS_INFO {
	vec2 TexCoords;
} vs_info[];

out vec2 TexCoords;

uniform float time;

vec3 GetNormal() {
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[1].gl_Position) - vec3(gl_in[2].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
	float magnitude = 2.0;
	vec3 dir = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return position + vec4(dir, 1.0);
}

void build_model() {
	vec3 normal = GetNormal();

	gl_Position = explode(gl_in[0].gl_Position, normal); // bottom-left
	TexCoords = vs_info[0].TexCoords;
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, normal); // bottom-right
	TexCoords = vs_info[1].TexCoords;
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, normal); // bottom-right
	TexCoords = vs_info[2].TexCoords;
	EmitVertex();

	EndPrimitive();
}

void main() {
	build_model();
}
