#include"vao.h"
#include<iostream>

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &vaoID);
}

// Links a VBO Attribute such as a position or color to the VAO
void VAO::linkAttrib(VBO& vbo, const GLuint& location, const GLuint& numberOfComponent, 
	const GLenum& componentType, const GLboolean& normalized, GLsizeiptr stride, void* offset)
{
	vbo.bind();
	glVertexAttribPointer(location, numberOfComponent, componentType, normalized, stride, offset);
	glEnableVertexAttribArray(location);
	vbo.unbind();
}

// Binds the VAO
void VAO::bind()
{
	glBindVertexArray(vaoID);
}

void VAO::linkTexture(const GLenum& target, const GLenum& texture, const unsigned int& textureData) {
	glActiveTexture(texture);
	glBindTexture(target, textureData);
}

void VAO::drawArrays(const GLenum& target, const int& first, const int& count) {
	glDrawArrays(target, first, count);
}

void VAO::drawArraysInstanced(const GLenum& target, const int& first, const int& count, const unsigned int& numberOfInstances) {
	glDrawArraysInstanced(target, first, count, numberOfInstances);
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