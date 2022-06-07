#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"camera.h"
#include"model.h"

#include"vao.h"
#include"vbo.h"
#include"fbo.h"
#include"texture_buffer.h"
#include"utils.h"

#include <iostream>

float points[] = {
	// position		// color
	-0.5f, 0.5f,	0.0f, 0.0f, 1.0f,
	 0.5f, 0.5f,	1.0f, 0.0f,	0.0f,
	 0.5f, -0.5f,	0.0f, 1.0f,	0.0f,
	-0.5f, -0.5f,	1.0f, 1.0f,	0.0f,
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
	GLFWwindow* window = glfwCreateWindow(Settings::SCR_WIDTH, Settings::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Create the window context
	glfwMakeContextCurrent(window);
	// Adjust the Viewport as the window is resized by registering the window to the callback
	glfwSetFramebufferSizeCallback(window, Utils::framebuffer_size_callback);
	// Get the mouse position offset by registering a mouse callback
	glfwSetCursorPosCallback(window, Utils::mouse_callback);
	// Register the mouse scroll callback
	glfwSetScrollCallback(window, Utils::scroll_callback);

	// Hide the cursor and capture it
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// Config OpenGL global state
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_PROGRAM_POINT_SIZE);

	/// Load package model
	Model ourModel("resources/objects/backpack/backpack.obj");

	Shader normalShader("vs.glsl", "fs.glsl", "gs.glsl");
	Shader modelShader("model-vs.glsl", "model-fs.glsl");

	// Get the uniform blocks
	unsigned int normalMatrices = glGetUniformBlockIndex(normalShader.ID, "Matrices");
	unsigned int modelMatrices = glGetUniformBlockIndex(modelShader.ID, "Matrices");
	// Bind uniform blocks to the binding point 0
	glUniformBlockBinding(normalShader.ID, normalMatrices, 0 /*binding point idx*/);
	glUniformBlockBinding(modelShader.ID, modelMatrices, 0);
	// Gen uniform buffer object
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Bind uniform buffer object to the binding point 0
	glBindBufferRange(GL_UNIFORM_BUFFER, 0 /*binding point idx*/, uboMatrices,
		0 /*offset*/, 2 * sizeof(glm::mat4) /*size of data*/);


	// draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		Settings::deltaTime = currentFrame - Settings::lastFrame;
		Settings::lastFrame = currentFrame;

		Utils::processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = Settings::camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 1.0f, 100.0f);

		// Set values for the uniform buffer object
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0 /*offset of the attr in the uniform block*/,
			sizeof(glm::mat4), glm::value_ptr(model));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) /*view matrix offset*/,
			sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Draw package model
		modelShader.use();
		modelShader.setMat4("projection", projection);
		ourModel.Draw(modelShader);

		normalShader.use();
		normalShader.setFloat("time", glfwGetTime());
		normalShader.setMat4("projection", projection);
		ourModel.Draw(normalShader);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
