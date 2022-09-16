#pragma once

#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Setting.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include <GLFW/glfw3.h>
#include<tuple>

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
	unsigned int Width, Height;
	// Game levels
	std::vector<GameLevel> Levels;
	unsigned int CurrentLevel;

	Game(unsigned int width, unsigned height);
	~Game();
	// Init game state
	void Init();
	// Game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	// Operations
	bool CheckCollisions(GameObject& obj1, GameObject& obj2);
	Collision CheckCollisions(GameObject& brick, BallObject& ball);
	void DoCollsions();
	Direction CalcCollidedDirection(glm::vec2 target);
	// Reset game
	void ResetLevel();
	void ResetPlayer();
private:
	SpriteRenderer* spriteRenderer;
	// Player
	GameObject* player;
	const glm::vec2 PLAYER_SIZE = glm::vec2(100.0f, 20.0f);
	const float PLAYER_VELOCITY = 500.0f;
	// Ball
	BallObject* ball;
	const float BALL_RADIUS = 12.5f;
	const glm::vec2 BALL_VELOCITY = glm::vec2(200.0f, -300.0f);
	// Particle generator
	ParticleGenerator* particleGenerator;
	void doBrickCollision();
	void doPlayerCollision();
	void initPlayer();
	void initBall();
};

