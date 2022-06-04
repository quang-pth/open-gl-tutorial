#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>

class EBO
{
public:
	// Reference ID of the Vertex Buffer Object
	GLuint eboID;
	// Constructor that generates a Vertex Buffer Object and links it to vertices
	EBO(float indicies[], const float& size);

	// Binds the VBO
	void bind();
	// Unbinds the VBO
	void unbind();
	// Deletes the VBO
	void deleteObj();
};

#endif