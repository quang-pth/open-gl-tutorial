#include "GameObject.h"

GameObject::GameObject() : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Sprite(), Color(1.0f), 
				Velocity(0.0f), IsSolid(false), Destroyed(false), Rotation(0.0)
{
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D& sprite,
	glm::vec3 color, glm::vec2 velocity) : IsSolid(false), Destroyed(false), Rotation(0.0)
{
	this->Position = pos;
	this->Size = size;
	this->Sprite = sprite;
	this->Color = color;
	this->Velocity = velocity;
}


GameObject::~GameObject()
{

}

void GameObject::Draw(SpriteRenderer& spriteRenderer)
{
	spriteRenderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation,
		this->Color);
}
