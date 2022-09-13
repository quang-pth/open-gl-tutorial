#include "BallObject.h"

BallObject::BallObject() : Radius(), IsStuck(true), IsMoving(false)
{
}

BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D& texture)
	: GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), texture),
	IsStuck(true), IsMoving(false)
{
	this->Velocity = velocity;
	this->Radius = radius;
}

BallObject::~BallObject()
{
}

glm::vec2 BallObject::Move(float dt, unsigned int windowWidth)
{
	if (this->IsStuck) return glm::vec2(0.0f, 0.0f);
	
	const glm::vec2 velocity = this->Velocity * dt;
	this->Position += velocity;

	if (this->Position.x <= 0) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = 0.0f;
	}
	else if (this->Position.x >= windowWidth - this->Size.x) {
		this->Velocity.x = -this->Velocity.x;
		this->Position.x = windowWidth - this->Size.x;
	}
	else if (this->Position.y <= 0) {
		this->Velocity.y = -this->Velocity.y;
		this->Position.y = 0.0f;
	}

	return this->Position;
}

void BallObject::Reset(glm::vec2 pos, glm::vec2 velocity)
{
}
