#include "Setting.h"

// Screen resolution
unsigned int Setting::SCR_WIDTH = 800;
unsigned int Setting::SCR_HEIGHT = 600;

// Name
std::string Setting::ballName = "awesomeface";
std::string Setting::backgroundName = "background";
std::string Setting::solidBrickName = "block_solid";
std::string Setting::normalBrickName = "block";
std::string Setting::levelShaderName = "levelShader";
std::string Setting::paddleName = "paddle";

// File paths
const char* Setting::ballFilePath = "Textures/awesomeface.png";
const char* Setting::backgroundFilePath = "Textures/background.jpg";
const char* Setting::solidBrickFilePath = "Textures/block_solid.png";
const char* Setting::normalBrickFilePath = "Textures/block.png";
const char* Setting::levelOneFilePath = "Levels/one.lvl";
const char* Setting::levelTwoFilePath = "Levels/two.lvl";
const char* Setting::levelThreeFilePath = "Levels/three.lvl";
const char* Setting::levelFourFilePath = "Levels/four.lvl";
const char* Setting::paddleFilePath= "Textures/paddle.png";
