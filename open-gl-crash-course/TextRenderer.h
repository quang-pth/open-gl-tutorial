#pragma once

#include<ft2build.h>
#include FT_FREETYPE_H

#include"Shader.h"
#include"Texture2D.h"
#include"ResourceManager.h"
#include"Setting.h"
#include<unordered_map>

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer
{
public:
	std::unordered_map<char, Character> Characters;
	Shader shader;
	TextRenderer(unsigned int width = 0, unsigned int height = 0);
	void LoadFont(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
private:
	unsigned int VAO, VBO;
};

