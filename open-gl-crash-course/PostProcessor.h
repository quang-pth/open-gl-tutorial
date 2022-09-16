#pragma once

#include"Shader.h"
#include"Texture2D.h"

class PostProcessor
{
public:
	Shader shader;
	Texture2D texture;
	unsigned int Width, Height;
	bool Chaos, Confuse, Shake;
	PostProcessor(Shader& shader, unsigned int width, unsigned int height);
	void BeginRender();
	void EndRender();
	void Render(float time);
private:
	unsigned int multiSampledFBO, FBO;
	unsigned int RBO;
	unsigned int VAO;
	void initRenderData();
};

