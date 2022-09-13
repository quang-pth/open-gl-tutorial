#include "Game.h"

Game::Game(unsigned int width, unsigned height) : Keys(), spriteRenderer(), Levels(), CurrentLevel()
{
	this->Width = width;
	this->Height = height;
	this->State = GAME_ACTIVE;
	//std::fill_n(this->Keys, 1024, 1);
}

Game::~Game()
{
	glDeleteShader(ResourceManager::GetShader(Setting::levelShaderName).ID);
	delete spriteRenderer;
}

void Game::Init()
{
	// Generate shaders
	Shader levelShader = ResourceManager::LoadShader("sprite_vs.glsl", "sprite_fs.glsl", 
		nullptr, Setting::levelShaderName);
	glm::mat4 orthoProjection = glm::ortho(0.0f, (float) this->Width, (float) this->Height, 0.0f, -1.0f, 1.0f);
	levelShader.Use();
	levelShader.setInt("image", 0);
	levelShader.setMat4("projection", orthoProjection);
	// Load game level textures
	ResourceManager::LoadTexture(Setting::backgroundFilePath, false, Setting::backgroundName);
	ResourceManager::LoadTexture(Setting::faceFilePath, true, Setting::faceName);
	ResourceManager::LoadTexture(Setting::solidBrickFilePath, false, Setting::solidBrickName);
	ResourceManager::LoadTexture(Setting::normalBrickFilePath, false, Setting::normalBrickName);
	ResourceManager::LoadTexture(Setting::paddleFilePath, false, Setting::paddleName);
	// Init game attributes
	this->spriteRenderer = new SpriteRenderer(levelShader);
	// Init player
	glm::vec2 playerPosition(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	this->player = new GameObject(playerPosition, PLAYER_SIZE, ResourceManager::GetTexture(Setting::paddleName));
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
	// Process Player's input
	if (this->State == GAME_ACTIVE) {
		const float velocity = this->PLAYER_VELOCITY * dt;
		// Player can only move horizontally
		if (this->Keys[GLFW_KEY_A]) {
			if (this->player->Position.x >= 0) {
				this->player->Position.x -= velocity;
			}
		}
		else if (this->Keys[GLFW_KEY_D]) {
			if (this->player->Position.x <= this->Width - this->PLAYER_SIZE.x) {
				this->player->Position.x += velocity;
			}
		}
	}
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	// Draw game levels
	if (this->State == GAME_ACTIVE) {
		if (this->player == nullptr || this->spriteRenderer == nullptr) {
			std::cout << "ERROR::NULL_REFERCENCES_EXCEPTION in Game.cpp line 80" << std::endl;
			exit;
		}
		this->spriteRenderer->DrawSprite(ResourceManager::GetTexture(Setting::backgroundName),
				glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->CurrentLevel].Draw(*this->spriteRenderer);
		this->player->Draw(*spriteRenderer);
	}
}
