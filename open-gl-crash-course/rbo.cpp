#include "rbo.h"

RBO::RBO(unsigned int width, unsigned int height) {
	glGenRenderbuffers(1, &rboID);
	initRenderBuffer(width, height);
}

void RBO::initRenderBuffer(unsigned int width, unsigned int height) {
	this->bind();
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	// Attach the stencil buffer and depth buffer to the FBO
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	this->unbind();
}

void RBO::initRenderBufferMS(unsigned int width, unsigned int height) {
	this->bind();
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
	// Attach the stencil buffer and depth buffer to the FBO
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 4, width, height);
	this->unbind();
}

void RBO::bind() {
	glBindRenderbuffer(GL_RENDERBUFFER, rboID);
}

void RBO::unbind() {
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void RBO::deleteObj() {
	glDeleteRenderbuffers(1, &rboID);
}