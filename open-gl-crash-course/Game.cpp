#include "Game.h"

Game::Game(unsigned int width, unsigned height)
{
	this->Width = width;
	this->Height = height;
}

Game::~Game()
{
	glDeleteShader(ResourceManager::GetShader(shaderName).ID);
	glDeleteTextures(1, &ResourceManager::GetTexture(spriteName).ID);
}

void Game::Init()
{
	Shader shader = ResourceManager::LoadShader("sprite_vs.glsl", "sprite_fs.glsl", nullptr, shaderName);
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
		static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	shader.Use();
	shader.setInt("image", 0);
	shader.setMat4("projection", projection);
	this->spriteRenderer = new SpriteRenderer(shader);
	ResourceManager::LoadTexture("Textures/awesomeface.png", true, spriteName);
}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	glm::vec2 position = glm::vec2(200.0f, 200.0f);
	glm::vec2 size = glm::vec2(300.0f, 400.0f);
	float rotate = 45.0f;
	glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
	spriteRenderer->DrawSprite(ResourceManager::GetTexture(spriteName), position, size, 
		rotate, color);
}
