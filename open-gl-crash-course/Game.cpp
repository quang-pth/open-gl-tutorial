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
	// Shaders
	Shader levelShader = ResourceManager::LoadShader("sprite_vs.glsl", "sprite_fs.glsl", 
		nullptr, Setting::levelShaderName);
	glm::mat4 orthoProjection = glm::ortho(0.0f, (float) this->Width, (float) this->Height, 0.0f, -1.0f, 1.0f);
	levelShader.Use();
	levelShader.setInt("image", 0);
	levelShader.setMat4("projection", orthoProjection);
	// Textures
	ResourceManager::LoadTexture(Setting::backgroundFilePath, false, Setting::backgroundName);
	ResourceManager::LoadTexture(Setting::ballFilePath, true, Setting::ballName);
	ResourceManager::LoadTexture(Setting::solidBrickFilePath, false, Setting::solidBrickName);
	ResourceManager::LoadTexture(Setting::normalBrickFilePath, false, Setting::normalBrickName);
	ResourceManager::LoadTexture(Setting::paddleFilePath, false, Setting::paddleName);
	ResourceManager::LoadTexture(Setting::ballFilePath, false, Setting::ballName);
	// Init game attributes
	this->spriteRenderer = new SpriteRenderer(levelShader);
	// Init player
	glm::vec2 playerPosition(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	this->player = new GameObject(playerPosition, PLAYER_SIZE, ResourceManager::GetTexture(Setting::paddleName));
	// Init ball
	glm::vec2 ballPos = glm::vec2(this->Width / 2.0f - BALL_RADIUS, this->Height - PLAYER_SIZE.y - BALL_RADIUS);
	//glm::vec2 ballPos(this->Width / 2.0f, this->Height / 2.0f);
	this->ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture(Setting::ballName));
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

	this->CurrentLevel = 0;
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
				if (this->ball->IsStuck) {
					this->ball->Position.x -= velocity;
					this->ball->IsMoving = true;
				}
			}
		}
		else if (this->Keys[GLFW_KEY_D]) {
			if (this->player->Position.x <= this->Width - this->PLAYER_SIZE.x) {
				this->player->Position.x += velocity;
				if (this->ball->IsStuck) {
					this->ball->Position.x += velocity;
					this->ball->IsMoving = true;
				}
			}
		}
		else {
			this->ball->IsMoving = false;
		}
		
		if (this->Keys[GLFW_KEY_SPACE]) {
			if (!this->ball->IsStuck) return;

			if (!this->ball->IsMoving) {
				this->ball->Velocity.x = 0;
			}

			this->ball->IsStuck = false;
		}
	}
}

void Game::Update(float dt)
{
	this->ball->Move(dt, this->Width);
	this->DoCollsions();
}

void Game::Render()
{
	// Draw game levels
	if (this->State == GAME_ACTIVE) {
		if (this->player == nullptr || this->spriteRenderer == nullptr || this->ball == nullptr) {
			std::cout << "ERROR::NULL_REFERCENCES_EXCEPTION in Game.cpp line 80" << std::endl;
			exit;
		}
		this->spriteRenderer->DrawSprite(ResourceManager::GetTexture(Setting::backgroundName),
				glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->CurrentLevel].Draw(*this->spriteRenderer);
		this->player->Draw(*spriteRenderer);
		this->ball->Draw(*spriteRenderer);
	}
}

bool Game::CheckCollisions(GameObject& obj1, GameObject& obj2)
{
	bool xIsCollided = !((obj1.Position.x + obj1.Size.x < obj2.Position.x) ||
		(obj1.Position.x > obj2.Position.x + obj2.Size.x));
	bool yIsCollided = !((obj1.Position.y + obj1.Size.y < obj2.Position.y) ||
		(obj1.Position.y > obj2.Position.y + obj2.Size.y));

	return xIsCollided && yIsCollided;
}

bool Game::CheckCollisions(GameObject& brick, BallObject& ball)
{
	// Ball center vector
	glm::vec2 ballCenterPoint(ball.Position + ball.Radius);
	// Brick extents width and heights
	glm::vec2 brickExtentDim(brick.Size.x / 2.0f, brick.Size.y / 2.0f);
	glm::vec2 brickCenterPoint(brick.Position + brickExtentDim);
	// Distance vector
	glm::vec2 distanceVector = brickCenterPoint - ballCenterPoint;
	glm::vec2 clampedDistanceVector = glm::clamp(distanceVector, -brickExtentDim, brickExtentDim);
	// Closest point
	glm::vec2 closestPoint = brickCenterPoint + clampedDistanceVector;
	// Check is collided
	return glm::length(closestPoint - ballCenterPoint) < ball.Radius;
}

void Game::DoCollsions()
{
	GameLevel *currentGameLevel = &this->Levels[this->CurrentLevel];
	for (GameObject& brick : currentGameLevel->Bricks) {
		if (!brick.Destroyed) {
			if (this->CheckCollisions(brick, *this->ball)) {
				if (!brick.IsSolid) {
					brick.Destroyed = true;
				}
			}
		}
	}
}
