#pragma once

#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Setting.h"
#include "GameLevel.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

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
private:
	SpriteRenderer* spriteRenderer;
};

