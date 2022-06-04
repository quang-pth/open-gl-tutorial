#include"EBO.h"
#include <iostream>
using namespace std;

// Constructor that generates a Vertex Buffer Object and links it to vertices
EBO::EBO(float indicies[], const float& size)
{
	glGenBuffers(1, &this->eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STATIC_DRAW);
}

// Binds the VBO
void EBO::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboID);
}

// Unbinds the VBO
void EBO::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void EBO::deleteObj()
{
	glDeleteBuffers(1, &this->eboID);
}