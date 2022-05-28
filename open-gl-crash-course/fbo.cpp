#include "fbo.h"

FBO::FBO() {
	glGenFramebuffers(1, &fboID);
}

void FBO::linkRenderBuffer(RBO renderBuffer) {
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer.rboID);
}

void FBO::linkTexture(TextureBuffer texColorBuffer) {
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer.textureID, 0);
}

void FBO::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
}

void FBO::checkCompileFramebuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
}

void FBO::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::deleteObj() {
	glDeleteFramebuffers(1, &fboID);
}