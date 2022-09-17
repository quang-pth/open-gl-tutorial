#pragma once

#include"GameObject.h"

//glm::vec2 SIZE = glm::vec2(60.0f, 20.0f);
//glm::vec2 VELOCITY = glm::vec2(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
	std::string Type;
	float Duration;
	bool Activated;
	PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D& texture)
		: GameObject(position, glm::vec2(60.0f, 20.0f), texture, color, glm::vec2(0.0f, 150.0f)), Type(type), Duration(duration), Activated(false)
	{

	}

	PowerUp& operator=(const PowerUp&) = default;
};

