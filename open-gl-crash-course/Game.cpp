#include "Game.h"

Game::Game(unsigned int width, unsigned height) : Keys(), spriteRenderer(), Levels(), CurrentLevel()
{
	this->Width = width;
	this->Height = height;
	this->State = GAME_ACTIVE;
}

Game::~Game()
{
	glDeleteShader(ResourceManager::GetShader(Setting::levelShaderName).ID);
	delete spriteRenderer;
}

void Game::Init()
{
	Shader levelShader = ResourceManager::LoadShader("sprite_vs.glsl", "sprite_fs.glsl", 
		nullptr, Setting::levelShaderName);
	glm::mat4 orthoProjection = glm::ortho(0.0f, (float) this->Width, (float) this->Height, 0.0f, -1.0f, 1.0f);
	levelShader.Use();
	levelShader.setInt("image", 0);
	levelShader.setMat4("projection", orthoProjection);
	this->spriteRenderer = new SpriteRenderer(levelShader);
	// Load game level textures
	ResourceManager::LoadTexture(Setting::backgroundFilePath, false, Setting::backgroundName);
	ResourceManager::LoadTexture(Setting::faceFilePath, true, Setting::faceName);
	ResourceManager::LoadTexture(Setting::solidBrickFilePath, false, Setting::solidBrickName);
	ResourceManager::LoadTexture(Setting::normalBrickFilePath, false, Setting::normalBrickName);
	// Load levels
	GameLevel levelOne, levelTwo, levelThree, levelFour;
	levelOne.LoadFromFile(Setting::levelOneFilePath, this->Width, this->Height / 2);
	levelTwo.LoadFromFile(Setting::levelTwoFilePath, this->Width, this->Height / 2);
	levelThree.LoadFromFile(Setting::levelThreeFilePath, this->Width, this->Height / 2);
	levelFour.LoadFromFile(Setting::levelFourFilePath, this->Width, this->Height / 2);
	// Add levels
	this->Levels.push_back(levelOne);
	this->Levels.push_back(levelTwo);
	this->Levels.push_back(levelThree);
	this->Levels.push_back(levelFour);

	this->CurrentLevel = 2;
}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	// Draw game levels
	if (this->State == GAME_ACTIVE) {
		if (spriteRenderer != nullptr) {
			this->spriteRenderer->DrawSprite(ResourceManager::GetTexture(Setting::backgroundName),
				glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
			this->Levels[this->CurrentLevel].Draw(*this->spriteRenderer);
		}
		else {
			std::cout << "NULL_EXCEPTION::ERROR::SPRITE RENDERER NULL OBJECT REFERENCES" << std::endl;
		}
	}
}
