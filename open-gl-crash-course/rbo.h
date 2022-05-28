#ifndef RBO_CLASS_H
#define RBO_CLASS_H

#include<glad/glad.h>
#include<iostream>

class RBO
{
public:
	unsigned int rboID;

	RBO(unsigned int width, unsigned int height);

	void initRenderBuffer(unsigned int width, unsigned int height);
	void bind();
	void unbind();
	void deleteObj();
};

#endif