#include"vao.h"
#include<iostream>

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &vaoID);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::linkAttrib(VBO& vbo, GLuint location, GLuint numberOfComponent, GLenum componentType, GLsizeiptr stride, void* offset)
{
	vbo.bind();
	glVertexAttribPointer(location, numberOfComponent, componentType, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(location);
	vbo.unbind();
}

// Binds the VAO
void VAO::bind()
{
	glBindVertexArray(vaoID);
}

void VAO::linkTexture(GLenum target, GLenum texture, unsigned int textureData) {
	glActiveTexture(texture);
	glBindTexture(target, textureData);
}

// Unbinds the VAO
void VAO::unbind()
{
	glBindVertexArray(0);
}

// Deletes the VAO
void VAO::deleteObj()
{
	glDeleteVertexArrays(1, &vaoID);
}