#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"camera.h"
#include "model.h"

#include"vao.h"
#include"vbo.h"
#include"fbo.h"
#include"texture_buffer.h"

#include <iostream>

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
	-0.5f, 0.5f,  0.5f, 0.0f, 0.0f, // bottom-left
	 0.5f, 0.5f,  0.5f, 1.0f, 0.0f, // bottom-right
	 0.5f, 0.5f, -0.5f, 1.0f, 1.0f, // top-right
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, // top-left
	 0.5f, 0.5f,  0.5f, 1.0f, 0.0f, // bottom-right
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
float quadVertices[] = {
	// positions // texCoords
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f
};

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
	// Light source shader
	Shader shader("framebuffer-vs.glsl", "framebuffer-fs.glsl");
	Shader screenShader("screen-framebuffer-vs.glsl", "screen-framebuffer-fs.glsl");

	// Cube
	VAO cubeVAO;
	VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
	cubeVAO.bind();
	cubeVAO.linkAttrib(cubeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	cubeVAO.linkAttrib(cubeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	cubeVAO.unbind();

	// Plane
	VAO planeVAO;
	VBO planeVBO(planeVertices, sizeof(planeVertices));
	planeVAO.bind();
	planeVAO.linkAttrib(planeVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
	planeVAO.linkAttrib(planeVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	planeVAO.unbind();

	// Quad
	VAO quadVAO;
	VBO quadVBO(quadVertices, sizeof(quadVertices));
	quadVAO.bind();
	quadVAO.linkAttrib(quadVBO, 0, 2, GL_FLOAT, 4 * sizeof(float), (void*)0);
	quadVAO.linkAttrib(quadVBO, 1, 2, GL_FLOAT, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	quadVAO.unbind();

	// Gen framebuffer
	FBO fbo;
	TextureBuffer texColorBuffer(SCR_WIDTH, SCR_HEIGHT);
	RBO rbo(SCR_WIDTH, SCR_HEIGHT);
	fbo.bind();
	fbo.linkTexture(texColorBuffer);
	fbo.linkRenderBuffer(rbo);
	fbo.checkCompileFramebuffer();
	fbo.unbind();

	// Cube texture (.jpg)
	unsigned int cubeTexture = loadTexture("Textures/container.jpg");
	// Floor texture (.png)
	unsigned int planeTexture = loadTexture("resources/textures/depth-testing/metal.png");

	shader.use();
	shader.setInt("Texture", 0);

	screenShader.use();
	screenShader.setInt("screenTexture", 0);
	
	// draw as wireframe
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		processInput(window);

		// Use our defined Framebuffer to draw screen
		camera.Yaw += 180.0f; // rotate camera
		camera.ProcessMouseMovement(0, 0, GL_FALSE);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom /*field of view*/), (float)SCR_WIDTH / (float)SCR_HEIGHT /*scene ration*/, 0.1f /*near plane*/, 100.0f /*far plane*/);
		glm::mat4 model = glm::mat4(1.0f);

		fbo.bind();
		glClearColor(0.1f, 0.1f, 0.1, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using function
		glEnable(GL_DEPTH_TEST);
		// Draw our first screen
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// cubes
		cubeVAO.bind();
		cubeVAO.linkTexture(GL_TEXTURE_2D, GL_TEXTURE0, cubeTexture);
		// First cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Second cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();
		// plane
		planeVAO.bind();
		planeVAO.linkTexture(GL_TEXTURE_2D, GL_TEXTURE0, planeTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		planeVAO.unbind();

		// Draw second screen with default frame buffer
		fbo.unbind();
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using function
		// reset camera
		camera.Yaw -= 180.0f;
		camera.ProcessMouseMovement(0, 0, GL_TRUE);
		view = camera.GetViewMatrix();
		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// cubes
		cubeVAO.bind();
		cubeVAO.linkTexture(GL_TEXTURE_2D, GL_TEXTURE0, cubeTexture);
		// First cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// Second cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		cubeVAO.unbind();
		// plane
		planeVAO.bind();
		planeVAO.linkTexture(GL_TEXTURE_2D, GL_TEXTURE0, planeTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		planeVAO.unbind();
		// Draw quad
		glDisable(GL_DEPTH_TEST);
		screenShader.use();
		quadVAO.bind();
		quadVAO.linkTexture(GL_TEXTURE_2D, GL_TEXTURE0, texColorBuffer.textureID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		quadVAO.unbind();

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();

		lastFrame = currentFrame;
	}

	// Clear Objects
	cubeVAO.deleteObj();
	planeVBO.deleteObj();
	quadVAO.deleteObj();
	cubeVBO.deleteObj();
	planeVBO.deleteObj();
	quadVBO.deleteObj();

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
