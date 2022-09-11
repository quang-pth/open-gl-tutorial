#pragma once

#include "SpriteRenderer.h"
#include "ResourceManager.h"

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
	std::string shaderName = "sprite";
	std::string spriteName = "sprite";
};

