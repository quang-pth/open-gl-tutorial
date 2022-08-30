#ifndef FBO_CLASS_H
#define FBO_CLASS_H

#include"texture_buffer.h"
#include"rbo.h"

#include<glad/glad.h>
#include<iostream>

class FBO
{
public:
	GLuint fboID;
	
	FBO();

	void linkRenderBuffer(const RBO& renderBuffer);
	void linkTexture(const TextureBuffer& texColorBuffer);
	void bind();
	void checkCompileFramebuffer();
	void unbind();
	void deleteObj();
};

#endif