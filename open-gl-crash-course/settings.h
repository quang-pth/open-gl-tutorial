#ifndef SETTINGS_H
#define SETTINGS_H

#include"camera.h"

class Settings {
public:
	static const unsigned int SCR_WIDTH = 800;
	static const unsigned int SCR_HEIGHT = 600;
	// Cursor init position
	static float lastX;
	static float lastY;

	static Camera camera;
	static bool firstMouse;

	static float deltaTime; // time between current frame and last frame
	static float lastFrame; // time of the last frame

	// Light Source Position
	static glm::vec3 lightPos;
};

#endif