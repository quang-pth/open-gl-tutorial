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

	Shader redShader("common-vs.glsl", "red-fs.glsl");
	Shader greenShader("common-vs.glsl", "green-fs.glsl");
	Shader blueShader("common-vs.glsl", "blue-fs.glsl");
	Shader yellowShader("common-vs.glsl", "yellow-fs.glsl");

	// Red
	VAO redVAO;
	VBO redVBO(cubeVertices, sizeof(cubeVertices));
	redVAO.bind();
	redVAO.linkAttrib(redVBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	redVAO.unbind();
	// Green
	VAO greenVAO;
	VBO greenVBO(cubeVertices, sizeof(cubeVertices));
	greenVAO.bind();
	greenVAO.linkAttrib(greenVBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	greenVAO.unbind();
	// Blue
	VAO blueVAO;
	VBO blueVBO(cubeVertices, sizeof(cubeVertices));
	blueVAO.bind();
	blueVAO.linkAttrib(blueVBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	blueVAO.unbind();
	// Yellow
	VAO yellowVAO;
	VBO yellowVBO(cubeVertices, sizeof(cubeVertices));
	yellowVAO.bind();
	yellowVAO.linkAttrib(yellowVBO, 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	yellowVAO.unbind();

	// Get the uniform blocks
	unsigned int red = glGetUniformBlockIndex(redShader.ID, "Matrices");
	unsigned int green = glGetUniformBlockIndex(greenShader.ID, "Matrices");
	unsigned int blue = glGetUniformBlockIndex(blueShader.ID, "Matrices");
	unsigned int yellow = glGetUniformBlockIndex(yellowShader.ID, "Matrices");
	// Bind uniform blocks to the binding point 0
	glUniformBlockBinding(redShader.ID, red, 0 /*binding point idx*/);
	glUniformBlockBinding(greenShader.ID, green, 0);
	glUniformBlockBinding(blueShader.ID, blue, 0);
	glUniformBlockBinding(yellowShader.ID, yellow, 0);
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
		Utils::processInput(window);
		glClearColor(0.1f, 0.1f, 0.1, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our defined Framebuffer to draw screen
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = Settings::camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(Settings::camera.Zoom /*field of view*/), 
			(float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT /*scene ration*/, 
			0.1f /*near plane*/, 100.0f /*far plane*/);
		
		// Set values for the uniform buffer object
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0 /*offset of the attr in the uniform block*/,
			sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) /*view matrix offset*/,
			sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Red cube
		redShader.use();
		model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
		redShader.setMat4("model", model);
		redVAO.drawArrays(GL_TRIANGLES, 0, 36);
		// Green cube
		greenShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
		greenShader.setMat4("model", model);
		greenVAO.drawArrays(GL_TRIANGLES, 0, 36);
		// Blue cube
		blueShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
		blueShader.setMat4("model", model);
		blueVAO.drawArrays(GL_TRIANGLES, 0, 36);
		// Yellow cube
		yellowShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
		yellowShader.setMat4("model", model);
		yellowVAO.drawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();

		Settings::lastFrame = currentFrame;
	}

	// Clear Objects
	redVAO.deleteObj();
	redVBO.deleteObj();

	glfwTerminate();
	return 0;
}
