#pragma once

#include"GameObject.h"

class BallObject : public GameObject
{
public:
	float Radius;
	bool IsStuck;
	bool IsMoving;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D& texture);
	~BallObject();

	glm::vec2 Move(float dt, unsigned int windowWidth);
	void Reset(glm::vec2 pos, glm::vec2 velocity);
};

