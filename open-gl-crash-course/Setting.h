#pragma once

#include<iostream>

class Setting
{
public:
	// Screen resolution
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	// Name
	static std::string ballName;
	static std::string backgroundName;
	static std::string solidBrickName;
	static std::string normalBrickName;
	static std::string levelShaderName;
	static std::string paddleName;
	static std::string particleName;
	static std::string powerUpChaosName;
	static std::string powerUpConfuseName;
	static std::string powerUpIncreaseName;
	static std::string powerUpPassthroughName;
	static std::string powerUpSpeedName;
	static std::string powerUpStickyName;
	// File paths
	static const char* ballFilePath;
	static const char* backgroundFilePath;
	static const char* solidBrickFilePath;
	static const char* normalBrickFilePath;
	static const char* levelOneFilePath;
	static const char* levelTwoFilePath;
	static const char* levelThreeFilePath;
	static const char* levelFourFilePath;
	static const char* paddleFilePath;
	static const char* particleFilePath;
	static const char* powerUpChaosFilePath;
	static const char* powerUpConfuseFilePath;
	static const char* powerUpIncreaseFilePath;
	static const char* powerUpPassthroughFilePath;
	static const char* powerUpSpeedFilePath;
	static const char* powerUpStickyFilePath;
	// In game music, sounds
	static const char* backgroundMusicFilePath;
	static const char* solidSoundFilePath;
	static const char* nonSolidSoundFilePath;
	static const char* paddleCollidedSoundFilePath;
	static const char* powerUpSoundFilePath;
	// Shader paths
	static const char* particleVS;
	static const char* particleFS;
	static const char* postProcessingVS;
	static const char* postProcessingFS;
	static const char* postProcessingShaderName;
	static const char* textRenderingVS;
	static const char* textRenderingFS;
	static const char* textRenderingShaderName;
	// Font
	static const char* ocraextFont;
private:
	Setting() {};
};

