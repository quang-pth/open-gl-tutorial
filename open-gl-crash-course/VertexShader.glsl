#version 330 core

layout (location = 0) in vec3 aPos; // position attribute
layout (location = 1) in vec3 aColor; // color attribute

out vec3 ourColor;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); // add xOffset to move triangle by the x position
	ourColor = aColor;
}