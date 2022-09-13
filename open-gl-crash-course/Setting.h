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
private:
	Setting() {};
};

