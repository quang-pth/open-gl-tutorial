#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

class VBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint vboID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	VBO(const float vertices[], float size);

	// Binds the VBO
	void bind();
	// Unbinds the VBO
	void unbind();
	// Deletes the VBO
	void deleteObj();
};

#endif