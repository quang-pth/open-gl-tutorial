#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

// geometry shader recives input as a set of vertices
in VS_INFO {
	vec3 Normal;
} vs_info[];

uniform mat4 projection;

uniform float MAGNITUDE;

void GenerateLine(int index) {
	// first point position
	gl_Position = projection * gl_in[index].gl_Position;
	EmitVertex();
	// second point position
	gl_Position = projection * (gl_in[index].gl_Position + vec4(vs_info[index].Normal, 0.0) * (sin(MAGNITUDE) * 0.1));
	EmitVertex();
	// Draw line 
	EndPrimitive();
}

void main() {
	GenerateLine(1);
	GenerateLine(2);
	GenerateLine(3);
}
