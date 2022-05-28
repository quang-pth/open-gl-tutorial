#ifndef TextureBuffer_CLASS_H
#define TextureBuffer_CLASS_H

#include<glad/glad.h>
#include<iostream>

class TextureBuffer
{
public:
	unsigned int textureID;

	TextureBuffer(int width, int height);

	void initTextureBuffer(int width, int height);
	void bind();
	void unbind();
	void deleteObj();
};

#endif