#pragma once

#include<iostream>

class Setting
{
public:
	// Screen resolution
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	// Name
	static std::string faceName;
	static std::string backgroundName;
	static std::string solidBrickName;
	static std::string normalBrickName;
	static std::string levelShaderName;
	// File paths
	static const char* faceFilePath;
	static const char* backgroundFilePath;
	static const char* solidBrickFilePath;
	static const char* normalBrickFilePath;
	static const char* levelOneFilePath;
	static const char* levelTwoFilePath;
	static const char* levelThreeFilePath;
	static const char* levelFourFilePath;
};

