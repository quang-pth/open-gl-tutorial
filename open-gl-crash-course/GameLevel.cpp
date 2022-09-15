#include "GameLevel.h"

GameLevel::GameLevel() : Bricks()
{

}

GameLevel::~GameLevel()
{
	std::vector<GameObject>().swap(Bricks);
}

void GameLevel::LoadFromFile(const char* filePath, unsigned int levelWidth, unsigned levelHeight)
{
	Bricks.clear();
	std::vector<std::vector<unsigned int>> tileData = ResourceManager::LoadLevel(filePath);
	if (tileData.size() > 0) {
		this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer& spriteRenderer)
{
	for (GameObject &brick : this->Bricks) {
		if (!brick.Destroyed) {
			brick.Draw(spriteRenderer);
		}
	}
}

bool GameLevel::IsCompleted()
{
	for (GameObject& brick : this->Bricks) {
		if (!brick.IsSolid && !brick.Destroyed) {
			return false;
		}
	}
	return true;
}

GameLevel GameLevel::ResetBricks()
{
	for (GameObject& brick : this->Bricks) {
		brick.Destroyed = false;
	}

	return *this;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	unsigned int height = tileData.size();
	unsigned int width = tileData[0].size();
	// Resize the tile size to fit on the screen
	float unitHeight = static_cast<float>(levelHeight) / height;
	float unitWidth = static_cast<float>(levelWidth) / width;
	// Init tiles
	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
			// Solid block
			if (tileData[row][col] == 1) {
				glm::vec2 pos(unitWidth * col, unitHeight * row);
				glm::vec2 size(unitWidth, unitHeight);
				glm::vec3 color(0.8f, 0.8f, 0.7f);
				GameObject tileObject(pos, size,
					ResourceManager::GetTexture(Setting::solidBrickName), color);
				tileObject.IsSolid = true;
				this->Bricks.push_back(tileObject);
			}
			// Normal block
			else if (tileData[row][col] > 1) {
				glm::vec3 color = glm::vec3(1.0f); // original: white
				if (tileData[row][col] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[row][col] == 3)
					color = glm::vec3(0.0f, 1.0f, 0.0f);
				else if (tileData[row][col] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[row][col] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unitWidth * col, unitHeight * row);
				glm::vec2 size(unitWidth, unitHeight);
				GameObject tileObject(pos, size,
					ResourceManager::GetTexture(Setting::normalBrickName), color);
				this->Bricks.push_back(tileObject);
			}
		}
	}
}
