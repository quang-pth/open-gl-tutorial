#pragma once

#include"GameObject.h"
#include"ResourceManager.h"
#include <vector>
#include"Setting.h"

class GameLevel
{
public:
	std::vector<GameObject> Bricks;
	GameLevel();
	~GameLevel();
	void LoadFromFile(const char* filePath, unsigned int levelWidth, unsigned levelHeight);
	void Draw(SpriteRenderer& spriteRenderer);
	bool IsCompleted();
private:
	void init(std::vector<std::vector<unsigned int>> tileData, 
		unsigned int levelWidth, unsigned int levelHeight);
};

