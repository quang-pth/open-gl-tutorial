#pragma once

#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Setting.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"
#include "TextRenderer.h"
#include <GLFW/glfw3.h>
#include<irrKlang/irrKlang.h>

#include<tuple>
#include<algorithm>

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	DIR_UP,
	DIR_RIGHT,
	DIR_DOWN,
	DIR_LEFT,
};

typedef	std::tuple<bool, Direction, glm::vec2> Collision;

class Game
{
public:
	GameState State;
	bool Keys[1024];
	bool KeysProcessed[1024];
	unsigned int Width, Height;
	std::vector<GameLevel> Levels;
	std::vector<PowerUp> PowerUps;
	unsigned int CurrentLevel;
	unsigned int Lives = 3;

	Game(unsigned int width, unsigned height);
	~Game();
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	bool CheckCollisions(GameObject& obj1, GameObject& obj2);
	Collision CheckCollisions(GameObject& brick, BallObject& ball);
	void DoCollsions();
	Direction CalcCollidedDirection(glm::vec2 target);
	void ResetLevel();
	void ResetPlayer();
	void ResetEffects();
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);
private:
	SpriteRenderer* spriteRenderer;
	GameObject* player;
	const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f);
	const float PLAYER_VELOCITY = 500.0f;
	BallObject* ball;
	const float BALL_RADIUS = 12.5f;
	const glm::vec2 BALL_VELOCITY = glm::vec2(200.0f, -300.0f);
	ParticleGenerator* particleGenerator;
	PostProcessor* postProcessor;
	TextRenderer* textRenderer;
	float shakeTime = 0.0f;
	void doBrickCollision();
	void doPlayerCollision();
	void initPlayer();
	void initBall();
	// Power up
	bool shouldSpawn(unsigned int chance);
	void activatePowerUp(PowerUp& powerUp);
	bool isOtherPowerUpActivating(std::string type);
	void drawPowerUps();
};

