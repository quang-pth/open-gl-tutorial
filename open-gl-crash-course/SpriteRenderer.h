#pragma once

#include "Shader.h"
#include "Texture2D.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class SpriteRenderer
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();
	void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 1.0f),
		float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	unsigned int quadVAO;
	Shader shader;
	void initRendererData();
};

