#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include"Shader.h"
#include"camera.h"
#include"stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void generateTexture();

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
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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

	// Build and Compile shader
	Shader lightingShader("VertexShader.glsl", "FragmentShader.glsl");
	Shader lightCubdeShader("LightSourceVertexShader.glsl", "LightSourceFragmentShader.glsl");

	// 36 Vertices Data to render a cube
	float vertices[] = {
		// Position				// Normal Vector perpendicular to each vertex
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// START BINDING CALLS
	unsigned int cubeVAO, VBO;
	// Create VAO to manage VBO and attributes pointers
	glGenVertexArrays(1, &cubeVAO);
	// Create Vertex Buffer Object (VBO) to manage Vertices Data
	glGenBuffers(1, &VBO); // generate VBO's id
	glBindVertexArray(cubeVAO); // bind the Vertex Array Object
	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Configure the POSITION vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex position attribute
	// Configure the NORMAL VECTOR vertex attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO
	glBindVertexArray(0); // unbind VAO
	// END BINDING CALLS

	// Create light source
	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Render loop
	while(!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;

		// Close GLFW when pressing Escape key
		processInput(window);

		// Clear color of the previous frame on the buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT /*clear depth info of the previous frame on the buffer*/); // state-using function

		glm::vec3 newLightPos = glm::vec3(lightPos.x * (float)sin(glfwGetTime()), lightPos.y * (float)cos(glfwGetTime()), lightPos.z * (float)cos(glfwGetTime()));
		// Draw CUBE
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", newLightPos);
		lightingShader.setVec3("viewPos", camera.Position);
		
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);

		float aspectRatio = (float) SCR_WIDTH / (float)SCR_HEIGHT;
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom) /*Camera FOV*/, aspectRatio, 0.1f /*distance to the near plane*/, 100.0f /*distance to the further plane*/);
		lightingShader.setMat4("projection", projection);
		
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Light Source
		model = glm::mat4(1.0f);
		model = glm::translate(model, newLightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightCubdeShader.use();
		lightCubdeShader.setMat4("model", model);
		lightCubdeShader.setMat4("view", view);
		lightCubdeShader.setMat4("projection", projection);
		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();

		lastFrame = currentFrame;
	}

	// de-allocate resources once the program is about to exit
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &cubeVAO);
	glDeleteBuffers(1, &lightCubeVAO);

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

void generateTexture() {
	// Create Texture object
	unsigned int texture[2];
	glGenTextures(2, texture);
	// Texture Object 1
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	// Set the texture wrapping parameters
	// S, T, R (3D texture) => x, y, z axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set mirror mode on texture x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set mirror mode on texture y axis
	// Set Texture Filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmap filtering for texture get scaled down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear for scale up
	// Load and generate the Texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0); // get image width, height and color channels
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0 /*Mipmap level*/,
			GL_RGB /*format to store the image*/, width, height, 0,
			GL_RGB /*format of the source image*/, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
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

	// Texture Object 2
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set mirror mode on texture x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set mirror mode on texture y axis
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmap filtering for texture get scaled down
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear for scale up
	// Load and generate the Texture
	data = stbi_load("Textures/awesomeface.png", &width, &height, &nrChannels, 0); // get image width, height and color channels
	if (data) {
		// png image has transparency and alpha channel => image source data-type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0 /*Mipmap level*/,
			GL_RGB /*format to store the image*/, width, height, 0,
			GL_RGBA /*format of the source image*/, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
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
}