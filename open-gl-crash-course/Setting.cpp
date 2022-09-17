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
std::string Setting::particleName = "particle";
std::string Setting::powerUpChaosName = "chaos";
std::string Setting::powerUpConfuseName = "confuse";
std::string Setting::powerUpIncreaseName = "increase";
std::string Setting::powerUpPassthroughName = "passthrough";
std::string Setting::powerUpSpeedName = "speed";
std::string Setting::powerUpStickyName = "sticky";

// File paths
const char* Setting::ballFilePath = "Textures/awesomeface.png";
const char* Setting::backgroundFilePath = "Textures/background.jpg";
const char* Setting::solidBrickFilePath = "Textures/block_solid.png";
const char* Setting::normalBrickFilePath = "Textures/block.png";
const char* Setting::levelOneFilePath = "Levels/one.lvl";
const char* Setting::levelTwoFilePath = "Levels/two.lvl";
const char* Setting::levelThreeFilePath = "Levels/three.lvl";
const char* Setting::levelFourFilePath = "Levels/four.lvl";
const char* Setting::paddleFilePath = "Textures/paddle.png";
const char* Setting::particleFilePath = "Textures/particle.png";
const char* Setting::powerUpChaosFilePath = "Textures/powerup_chaos.png";
const char* Setting::powerUpConfuseFilePath = "Textures/powerup_confuse.png";
const char* Setting::powerUpIncreaseFilePath = "Textures/powerup_increase.png";
const char* Setting::powerUpPassthroughFilePath = "Textures/powerup_passthrough.png";
const char* Setting::powerUpSpeedFilePath = "Textures/powerup_speed.png";
const char* Setting::powerUpStickyFilePath = "Textures/powerup_sticky.png";
// In game music, sounds
const char* Setting::backgroundMusicFilePath = "audio/breakout.mp3";
const char* Setting::solidSoundFilePath = "audio/solid.wav";
const char* Setting::nonSolidSoundFilePath = "audio/bleep.mp3";
const char* Setting::powerUpSoundFilePath = "audio/powerup.wav";
const char* Setting::paddleCollidedSoundFilePath = "audio/bleep.wav";

// Shader paths
const char* Setting::particleVS = "particle_vs.glsl";
const char* Setting::particleFS = "particle_fs.glsl";
const char* Setting::postProcessingVS = "pp_vs.glsl";
const char* Setting::postProcessingFS = "pp_fs.glsl";
const char* Setting::postProcessingShaderName = "postProcessing";
