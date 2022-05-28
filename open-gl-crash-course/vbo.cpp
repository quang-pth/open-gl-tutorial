#include"VBO.h"
#include <iostream>
using namespace std;

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(float vertices[], float size)
{
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

// Binds the VBO
void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
}

// Unbinds the VBO
void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::deleteObj()
{
	glDeleteBuffers(1, &vboID);
}