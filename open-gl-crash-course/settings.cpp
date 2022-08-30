#include"settings.h"

float Settings::lastX = Settings::SCR_WIDTH / 2;
float Settings::lastY = Settings::SCR_WIDTH / 2;

Camera Settings::camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool Settings::firstMouse = true;

float Settings::deltaTime = 0.0f; // time between current frame and last frame
float Settings::lastFrame = 0.0f; // time of the last frame

// Light Source Position
glm::vec3 Settings::lightPos = glm::vec3(1.2f, 5.0f, 5.0f);