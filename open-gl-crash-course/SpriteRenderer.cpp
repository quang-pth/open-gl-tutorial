#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
{
	this->shader = shader;
	this->initRendererData();
}

SpriteRenderer::~SpriteRenderer()
{

}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, 
	float rotate, glm::vec3 color)
{
	// Transform matrices
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(position, 0.0));
	model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.0));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.0));
	model = glm::scale(model, glm::vec3(size, 1.0));
	this->shader.Use();
	// Set shader uniform
	this->shader.setMat4("model", model);
	this->shader.setVec3("spriteColor", color);
	// Draw sprite
	glBindVertexArray(this->quadVAO);
	glActiveTexture(GL_TEXTURE0);
	//std::cout << texture.ID << std::endl;
	texture.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::initRendererData()
{
	float quadVertices[] = {
		// pos		// texcoords
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	unsigned int quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(this->quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
