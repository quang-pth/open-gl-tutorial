#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	// ID reference for the Vertex Array Object
	GLuint vaoID;
	// Constructor that generates a VAO ID
	VAO();

	// Links a VBO Attribute such as a position or color to the VAO
	void linkAttrib(VBO& VBO, const GLuint& location, const GLuint& numberOfComponent, 
		const GLenum& componentType, const GLboolean& normalized, GLsizeiptr stride, void* offset);
	// Binds the VAO
	void bind();
	// Binds the texture
	void linkTexture(const GLenum& target, const GLenum& texture, const unsigned int& textureData);
	// draw
	void drawArrays(const GLenum& target, const int& first, const int& count);
	// Unbinds the VAO
	void unbind();
	// Deletes the VAO
	void deleteObj();
};

#endif