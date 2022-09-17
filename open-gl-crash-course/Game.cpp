#include "Game.h"

#pragma comment(lib, "irrKlang.lib")

irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

Game::Game(unsigned int width, unsigned height) : Keys(), spriteRenderer(), Levels(), CurrentLevel(), ball(), player(), postProcessor()
{
	this->Width = width;
	this->Height = height;
	this->State = GAME_ACTIVE;
}

Game::~Game()
{
	std::vector<GameLevel>().swap(Levels);
	delete spriteRenderer;
	delete player;
	delete ball;
	delete postProcessor;
	delete particleGenerator;
	SoundEngine->drop();
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
	Shader particleShader = ResourceManager::LoadShader(Setting::particleVS, Setting::particleFS, nullptr, Setting::particleName);
	particleShader.Use();
	particleShader.setInt("sprite", 0);
	particleShader.setMat4("projection", orthoProjection);
	Shader postProcessingShader = ResourceManager::LoadShader(Setting::postProcessingVS, Setting::postProcessingFS, nullptr, Setting::postProcessingShaderName);
	postProcessingShader.Use();
	postProcessingShader.setInt("scene", 0);
	postProcessingShader.setMat4("projection", orthoProjection);
	// Textures
	ResourceManager::LoadTexture(Setting::backgroundFilePath, false, Setting::backgroundName);
	ResourceManager::LoadTexture(Setting::ballFilePath, true, Setting::ballName);
	ResourceManager::LoadTexture(Setting::solidBrickFilePath, false, Setting::solidBrickName);
	ResourceManager::LoadTexture(Setting::normalBrickFilePath, false, Setting::normalBrickName);
	ResourceManager::LoadTexture(Setting::paddleFilePath, false, Setting::paddleName);
	ResourceManager::LoadTexture(Setting::particleFilePath, true, Setting::particleName);
	ResourceManager::LoadTexture(Setting::powerUpChaosFilePath, true, Setting::powerUpChaosName);
	ResourceManager::LoadTexture(Setting::powerUpConfuseFilePath, true, Setting::powerUpConfuseName);
	ResourceManager::LoadTexture(Setting::powerUpIncreaseFilePath, true, Setting::powerUpIncreaseName);
	ResourceManager::LoadTexture(Setting::powerUpPassthroughFilePath, true, Setting::powerUpPassthroughName);
	ResourceManager::LoadTexture(Setting::powerUpSpeedFilePath, true, Setting::powerUpSpeedName);
	ResourceManager::LoadTexture(Setting::powerUpStickyFilePath, true, Setting::powerUpStickyName);
	// Init game attributes
	this->spriteRenderer = new SpriteRenderer(levelShader);
	this->particleGenerator = new ParticleGenerator(particleShader, ResourceManager::GetTexture(Setting::particleName));
	this->postProcessor = new PostProcessor(postProcessingShader, this->Width, this->Height);
	// Init game objects
	this->initPlayer();
	this->initBall();
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
	// Game music
	SoundEngine->play2D(Setting::backgroundMusicFilePath, true);
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
	if (this->ball->Position.y > Setting::SCR_HEIGHT) {
		this->ResetLevel();
		this->ResetPlayer();
		this->ResetEffects();
	}
	if (this->shakeTime > 0.0f) {
		this->shakeTime -= dt;
		if (this->shakeTime <= 0.0f) {
			this->postProcessor->Shake = false;
		}
	}
	this->particleGenerator->Update(dt, *this->ball, glm::vec2(this->ball->Radius / 2.0f));
	this->UpdatePowerUps(dt);
}

void Game::Render()
{
	// Draw game levels
	if (this->State == GAME_ACTIVE) {
		if (this->player == nullptr || this->spriteRenderer == nullptr || this->ball == nullptr || 
			this->particleGenerator == nullptr || this->postProcessor == nullptr) 
		{
			std::cout << "ERROR::NULL_REFERCENCES_EXCEPTION in Game.cpp function Render()" << std::endl;
			exit;
		}
		
		this->postProcessor->BeginRender();
		this->spriteRenderer->DrawSprite(ResourceManager::GetTexture(Setting::backgroundName),
				glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		this->Levels[this->CurrentLevel].Draw(*this->spriteRenderer);
		this->player->Draw(*spriteRenderer);
		// Powerups
		this->drawPowerUps();
		this->particleGenerator->Draw();
		this->ball->Draw(*spriteRenderer);
		this->postProcessor->EndRender();
		this->postProcessor->Render(glfwGetTime());
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

Collision Game::CheckCollisions(GameObject& brick, BallObject& ball)
{
	// Ball center vector
	glm::vec2 ballCenterPoint(ball.Position + ball.Radius);
	// Brick extents width and heights
	glm::vec2 brickExtentDim(brick.Size.x / 2.0f, brick.Size.y / 2.0f);
	glm::vec2 brickCenterPoint(brick.Position + brickExtentDim);
	// Distance vector
	glm::vec2 distanceVector = ballCenterPoint - brickCenterPoint;
	glm::vec2 clampedDistanceVector = glm::clamp(distanceVector, -brickExtentDim, brickExtentDim);
	// Closest point
	glm::vec2 closestPoint = brickCenterPoint + clampedDistanceVector;
	// Check is collided
	glm::vec2 difference = closestPoint - ballCenterPoint;
	bool isCollided = glm::length(difference) < ball.Radius;
	if (isCollided) {
		return std::make_tuple(isCollided, this->CalcCollidedDirection(difference), difference);
	}
	
	return std::make_tuple(false, DIR_UP, distanceVector);
}

void Game::DoCollsions()
{
	this->doBrickCollision();
	this->doPlayerCollision();
}

Direction Game::CalcCollidedDirection(glm::vec2 target)
{
	glm::vec2 basisVector[] = {
		glm::vec2(0.0f, 1.0f), // UP
		glm::vec2(1.0f, 0.0f), // RIGHT
		glm::vec2(0.0f, -1.0f), // DOWN 
		glm::vec2(-1.0f, 0.0f), // LEFT
	};
	// Find the target closest vector
	unsigned int closestVector = -1;
	float maxDotProduct = 0.0f;
	for (unsigned int i = 0; i < sizeof(basisVector) / sizeof(basisVector[0]); i++) {
		float dotProduct = glm::dot(glm::normalize(target), basisVector[i]);
		if (dotProduct > maxDotProduct) {
			maxDotProduct = dotProduct;
			closestVector = i;
		}
	}

	return (Direction)closestVector;
}

void Game::ResetLevel()
{
	if (this->State == GAME_ACTIVE && this->spriteRenderer != nullptr) {
		this->Levels[this->CurrentLevel].ResetBricks().Draw(*spriteRenderer);
	}
}

void Game::ResetPlayer()
{
	this->initPlayer();
	this->initBall();
}

void Game::ResetEffects()
{
	this->PowerUps.clear();
	this->postProcessor->Confuse = false;
	this->postProcessor->Chaos = false;
}

void Game::SpawnPowerUps(GameObject& block)
{
	unsigned int positiveChance = 40;
	unsigned int negativeChance = 20;
	// Paddle increase powerup
	if (this->shouldSpawn(positiveChance)) {
		PowerUps.push_back(PowerUp(Setting::powerUpIncreaseName, glm::vec3(1.0f, 0.6f, 0.4f),
			10.0f, block.Position, ResourceManager::GetTexture(Setting::powerUpIncreaseName)));
	}
	// Passthrough powerup
	else if (this->shouldSpawn(positiveChance)) {
		PowerUps.push_back(PowerUp(Setting::powerUpPassthroughName, glm::vec3(0.5f, 1.0f, 0.5f),
			30.0f, block.Position, ResourceManager::GetTexture(Setting::powerUpPassthroughName)));
	}
	// Speed powerup
	else if (this->shouldSpawn(positiveChance)) {
		PowerUps.push_back(PowerUp(Setting::powerUpSpeedName, glm::vec3(0.5f, 0.5f, 1.0f),
			15.0f, block.Position, ResourceManager::GetTexture(Setting::powerUpSpeedName)));
	}
	// Sticky powerup
	else if (this->shouldSpawn(positiveChance)) {
		PowerUps.push_back(PowerUp(Setting::powerUpStickyName, glm::vec3(1.0f, 0.5f, 1.0f),
			20.0f, block.Position, ResourceManager::GetTexture(Setting::powerUpStickyName)));
	}
	// Chaos effect
	else if (this->shouldSpawn(negativeChance)) {
		PowerUps.push_back(PowerUp(Setting::powerUpChaosName, glm::vec3(0.9f, 0.25f, 0.25f),
			10.0f, block.Position, ResourceManager::GetTexture(Setting::powerUpChaosName)));
	}
	// Confuse effect
	else if (this->shouldSpawn(negativeChance)) {
		PowerUps.push_back(PowerUp(Setting::powerUpConfuseName, glm::vec3(1.0f, 0.3f, 0.3f),
			10.0f, block.Position, ResourceManager::GetTexture(Setting::powerUpConfuseName)));
	}
}

void Game::UpdatePowerUps(float dt)
{
	for (PowerUp& powerUp : this->PowerUps) {
		powerUp.Position += powerUp.Velocity * dt;
		
		if (powerUp.Position.y >= this->Height) {
			powerUp.Destroyed = true;
		}

		if (!powerUp.Activated) {
			continue;
		}

		powerUp.Duration -= dt;
		if (powerUp.Duration <= 0) {
			powerUp.Activated = false;
			if (powerUp.Type == Setting::powerUpStickyName) {
				if (!this->isOtherPowerUpActivating(Setting::powerUpStickyName)) {
					this->ball->Sticky = false;
					this->ball->Color = glm::vec3(1.0f);
				}
			}
			else if (powerUp.Type == Setting::powerUpIncreaseName) {
				if (!this->isOtherPowerUpActivating(Setting::powerUpIncreaseName)) {
					this->player->Size.x -= 50;
					this->ball->Color = glm::vec3(1.0f);
				}
			}
			else if (powerUp.Type == Setting::powerUpSpeedName) {
				if (!this->isOtherPowerUpActivating(Setting::powerUpSpeedName)) {
					this->ball->Velocity = glm::length(this->BALL_VELOCITY) * glm::normalize(this->ball->Velocity);
					this->ball->Color = glm::vec3(1.0f);
				}
			}
			else if (powerUp.Type == Setting::powerUpPassthroughName) {
				if (!this->isOtherPowerUpActivating(Setting::powerUpPassthroughName)) {
					this->ball->Passthrough = false;
					this->ball->Color = glm::vec3(1.0f);
				}
			}
			else if (powerUp.Type == Setting::powerUpConfuseName) {
				if (!this->isOtherPowerUpActivating(Setting::powerUpConfuseName)) {
					this->postProcessor->Confuse = false;
				}
			}
			else if (powerUp.Type == Setting::powerUpChaosName) {
				if (!this->isOtherPowerUpActivating(Setting::powerUpChaosName)) {
					this->postProcessor->Chaos = false;
				}
			}
		}
	}

	this->PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
		[] (PowerUp& powerUp) {
			return powerUp.Destroyed && !powerUp.Activated;
	}
	), this->PowerUps.end());
}

void Game::doBrickCollision()
{
	GameLevel* currentGameLevel = &this->Levels[this->CurrentLevel];
	// Calculate brick collision
	for (GameObject& brick : currentGameLevel->Bricks) {
		if (!brick.Destroyed) {
			Collision collision = CheckCollisions(brick, *this->ball);
			bool isCollided = std::get<0>(collision);
			
			if (isCollided) {
				if (!brick.IsSolid) {
					brick.Destroyed = true;
					this->SpawnPowerUps(brick);
					SoundEngine->play2D(Setting::nonSolidSoundFilePath, false);
				}
				else {
					this->shakeTime = 0.05f;
					this->postProcessor->Shake = true;
					SoundEngine->play2D(Setting::solidSoundFilePath, false);
				}

				if (this->ball->Passthrough && !brick.IsSolid) {
					continue;
				}

				Direction collidedDirection = std::get<1>(collision);
				glm::vec2 collidedVector = std::get<2>(collision);

				if (collidedDirection == DIR_LEFT || collidedDirection == DIR_RIGHT) {
					this->ball->Velocity.x = -this->ball->Velocity.x;

					float penetration = this->ball->Radius - std::abs(collidedVector.x);
					if (collidedDirection == DIR_LEFT) {
						this->ball->Position.x -= penetration;
					}
					else {
						this->ball->Position.x += penetration;
					}
				}
				else if (collidedDirection == DIR_UP || collidedDirection == DIR_DOWN) {
					this->ball->Velocity.y = -this->ball->Velocity.y;

					float penetration = this->ball->Radius - std::abs(collidedVector.y);
					if (collidedDirection == DIR_UP) {
						this->ball->Position.y -= penetration;
					}
					else {
						this->ball->Position.y += penetration;
					}
				}

			}
		}
	}
	// Calculate powerup collision
	for (PowerUp& powerUp : this->PowerUps) {
		if (!powerUp.Destroyed) {
			if (this->CheckCollisions(*this->player, powerUp)) {
				this->activatePowerUp(powerUp);
				powerUp.Destroyed = true;
				powerUp.Activated = true;
				SoundEngine->play2D(Setting::powerUpSoundFilePath, false);
			}
		}
	}
}

void Game::doPlayerCollision()
{
	Collision collision = this->CheckCollisions(*this->player, *this->ball);
	bool isCollided = std::get<0>(collision);
	if (isCollided && !this->ball->IsStuck) {
		this->ball->IsStuck = this->ball->Sticky;
		float playerCenterPoint = this->player->Position.x + this->player->Size.x / 2.0f;
		float distanceFromCenter = std::abs(this->ball->Position.x + this->ball->Radius - playerCenterPoint);
		float pecentage = distanceFromCenter / (this->player->Size.x / 2.0f);
		float strength = 2.0f;
		glm::vec2 oldVelocity = this->ball->Velocity;
		glm::vec2 newVelocity(BALL_VELOCITY.x * strength * pecentage, -1.0f * std::abs(this->ball->Velocity.y));
		this->ball->Velocity = glm::normalize(newVelocity) * glm::length(oldVelocity);
		SoundEngine->play2D(Setting::paddleCollidedSoundFilePath, false);
	}
}

void Game::initPlayer()
{
	glm::vec2 playerPosition(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	this->player = new GameObject(playerPosition, PLAYER_SIZE, ResourceManager::GetTexture(Setting::paddleName));
}

void Game::initBall()
{
	glm::vec2 ballPos = glm::vec2(this->Width / 2.0f - BALL_RADIUS, this->Height - PLAYER_SIZE.y - BALL_RADIUS);
	this->ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture(Setting::ballName));
}

bool Game::shouldSpawn(unsigned int chance)
{
	unsigned int random = rand() % chance;
	return random == 0;
}

void Game::activatePowerUp(PowerUp& powerUp)
{
	if (powerUp.Type == Setting::powerUpSpeedName) {
		this->ball->Velocity *= 1.2;
	}
	else if (powerUp.Type == Setting::powerUpStickyName) {
		this->ball->Sticky = true;
		this->player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == Setting::powerUpPassthroughName) {
		this->ball->Passthrough = true;
		this->player->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == Setting::powerUpIncreaseName) {
		if (!this->isOtherPowerUpActivating(Setting::powerUpIncreaseName)) {
			this->player->Size.x += 50;
		}
	}
	else if (powerUp.Type == Setting::powerUpChaosName) {
		if (!this->postProcessor->Confuse) {
			this->postProcessor->Chaos = true;
		}
	}
	else if (powerUp.Type == Setting::powerUpConfuseName) {
		if (!this->postProcessor->Chaos) {
			this->postProcessor->Confuse = true;
		}
	}
}

bool Game::isOtherPowerUpActivating(std::string type)
{
	for (PowerUp& powerUp : this->PowerUps) {
		if (powerUp.Activated && powerUp.Type == type) {
			return true;
		}
	}

	return false;
}

void Game::drawPowerUps()
{
	for (PowerUp& powerUp : this->PowerUps) {
		if (!powerUp.Destroyed) {
			powerUp.Draw(*this->spriteRenderer);
		}
	}
}
