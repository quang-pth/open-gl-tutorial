#include "Texture2D.h"

Texture2D::Texture2D()
{
	this->Width = 0;
	this->Height = 0;
	this->InternalFormat = GL_RGB;
	this->ImageFormat = GL_RGB;
	this->WrapS = GL_REPEAT;
	this->WrapT = GL_REPEAT;
	this->FilterMin = GL_LINEAR;
	this->FilterMax = GL_LINEAR;
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(unsigned int width, unsigned int height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->InternalFormat, width, height, 0, 
		this->ImageFormat, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilterMin);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilterMax);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
