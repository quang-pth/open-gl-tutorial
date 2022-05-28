#include "texture_buffer.h"

TextureBuffer::TextureBuffer(int width, int height) {
	glGenTextures(1, &textureID);
	initTextureBuffer(width, height);
}

void TextureBuffer::initTextureBuffer(int width, int height) {
	this->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL /*we just alocate a memory for this texture - not fill it*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->unbind();
}

void TextureBuffer::bind() {
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void TextureBuffer::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureBuffer::deleteObj() {
	glDeleteTextures(1, &textureID);
}
