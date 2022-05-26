#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"camera.h"
#include "model.h"

#include <iostream>
#include <map>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path, const char* name = NULL);
void setPointLight(Shader &shader, int index, glm::vec3 position, glm::vec3 color);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cursor init position
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

// Camera setup
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
Camera camera(cameraPosition);

bool firstMouse = true;

float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of the last frame

// Light Source Position
glm::vec3 lightPos(1.2f, 5.0f, 5.0f);

int main() {
	glfwInit();
	/*
		Configuring GLFW:
		Use OpenGL version 3.3
		Use the core-profile
	*/
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create GLFW window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Create the window context
	glfwMakeContextCurrent(window);
	// Adjust the Viewport as the window is resized by registering the window to the callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// Get the mouse position offset by registering a mouse callback
	glfwSetCursorPosCallback(window, mouse_callback);
	// Register the mouse scroll callback
	glfwSetScrollCallback(window, scroll_callback);

	// Hide the cursor and capture it
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Enable blending state
	glEnable(GL_BLEND);
	// Source color: fragment output color
	// Destination color: color on color buffer
	// => Auto set by OpenGL
	glBlendFunc(GL_SRC_ALPHA /*Set source factor equal to the source color alpha*/, 
		GL_ONE_MINUS_SRC_ALPHA /*Set destination factor equal to 1 - source color alpha*/);
	
	// Enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK /*discard back-facing fragments*/);
	glFrontFace(GL_CCW /*set triangle with counter-clockwise winding order as front-facing face*/);

	// Light source shader
	Shader shader("depth-test-vertex.glsl", "depth-test-fragment.glsl");
	Shader blendingShader("blending-vertex.glsl", "blending-fragment.glsl");

	float cubeVertices[] = {
		// back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // top-left
		// front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // top-right
		 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // top-right
		-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // bottom-left
		// left face
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-right
		// right face
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
		0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // bottom-right
		0.5f,  0.5f,  0.5f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-left
		// bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, // top-left
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top-right
		// top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		 0.5f, 0.5f,  0.5f, 1.0f, 0.0f, // bottom-right
		 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
		 0.5f, 0.5f,  0.5f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
		-0.5f, 0.5f,  0.5f, 0.0f, 0.0f // bottom-left
	};
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		-5.0f, -0.501f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.501f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.501f,  5.0f,  0.0f, 0.0f,

		 5.0f, -0.501f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.501f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.501f, -5.0f,  2.0f, 2.0f
	};
	float blendingObjVertices[] = {
		// positions  // texture Coords
		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  1.0f
	};

	// Cube 
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	// config VBO
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// cube position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	// cube texture coords attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// unbind vbo and vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// Plane
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coords attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Window
	unsigned int blendingObjVAO, blendingObjVBO;
	glGenVertexArrays(1, &blendingObjVAO);
	glGenBuffers(1, &blendingObjVBO);
	glBindVertexArray(blendingObjVAO);
	glBindBuffer(GL_ARRAY_BUFFER, blendingObjVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(blendingObjVertices), blendingObjVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Cube texture (.jpg)
	unsigned int cubeTexture = loadTexture("resources/textures/depth-testing/marble.jpg");
	// Floor texture (.png)
	unsigned int floorTexture = loadTexture("resources/textures/depth-testing/metal.png");
	// Semi-transparent window texture (.png)
	unsigned int windowTexture = loadTexture("resources/textures/blending/blending_transparent_window.png", "change-wrapping");

	// Windows positions
	vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windows.push_back(glm::vec3(-1.5f, 0.0f, -0.51f));
	windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	shader.use();
	shader.setInt("Texture", 0);
	blendingShader.use();
	blendingShader.setInt("Texture", 0);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

		// Close GLFW when pressing Escape key
		processInput(window);

		// Clear color of the previous frame on the buffer
		glClearColor(0.1f, 0.1f, 0.1, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*clear depth info of the previous frame on the buffer*/); // state-using function
		
		
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom /*field of view*/), (float)SCR_WIDTH / (float)SCR_HEIGHT /*scene ration*/, 0.1f /*near plane*/, 100.0f /*far plane*/);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		
		// cubes
		glEnable(GL_CULL_FACE);
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		// First cube
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Secone cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		// plane
		glDisable(GL_CULL_FACE);
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// sort windows by distance
		std::map<float, glm::vec3> sortedPositions;
		for (unsigned int i = 0; i < windows.size(); i++) {
			float distance = glm::length(camera.Position - windows[i]);
			sortedPositions[distance] = windows[i]; // map sorts elements by key
		}
		// draw windows
		blendingShader.use();
		glBindVertexArray(blendingObjVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, windowTexture);
		blendingShader.setMat4("view", view);
		blendingShader.setMat4("projection", projection);
		for (std::map<float, glm::vec3>::reverse_iterator pos = sortedPositions.rbegin(); pos != sortedPositions.rend(); ++pos) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pos->second);
			blendingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();

		lastFrame = currentFrame;
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	/*
	=============================================
		START Control Camera
	=============================================
	*/
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	/*
	=============================================
		END Control Camera
	=============================================
	*/
}

void setPointLight(Shader& shader, int index, glm::vec3 position, glm::vec3 color) {
	string prefix = "pointLight[" + to_string(index) + "]";
	// Set light point attributes
	shader.setVec3(prefix + ".position", position);
	shader.setVec3(prefix + ".ambient", color * 0.1f);
	shader.setVec3(prefix + ".diffuse", color * 1.3232f);
	shader.setVec3(prefix + ".specular", color * 0.08f);
	shader.setFloat(prefix + ".constant", 1.0f);
	shader.setFloat(prefix + ".linear", 0.07f);
	shader.setFloat(prefix + ".quadratic", 0.017f);
}

unsigned int loadTexture(const char* path, const char* name) {
	// Create Texture object
	unsigned int textureID;
	glGenTextures(1, &textureID);
	// Load and generate the Texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); // get image width, height and color channels

	if (data) {
		// Set texture color channels
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		// Set texture wrapping format
		GLenum wrappingFormat = GL_REPEAT;
		if (name == "change-wrapping") wrappingFormat = GL_CLAMP_TO_EDGE;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0 /*Mipmap level*/,
			format /*format to store the image*/, width, height, 0,
			format /*format of the source image*/, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the texture wrapping parameters
		// S, T, R (3D texture) => x, y, z axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingFormat); // set mirror mode on texture x axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingFormat); // set mirror mode on texture y axis
		// Set Texture Filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmap filtering for texture get scaled down
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear for scale up
	}
	else {
		std::cout << "====================TEXTURE==================" << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "nrChannels: " << nrChannels << std::endl;
		std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
		std::cout << "====================TEXTURE==================" << std::endl;
	}
	// Free the image memory
	stbi_image_free(data);

	return textureID;
}