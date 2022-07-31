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
	glDepthFunc(GL_LESS);
	// Stencil testing
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP /*keep the stencil's content if stencil testing fail*/,
		GL_KEEP /*keep the stencil's content if depth testing fail*/,
		GL_REPLACE /*replace the stencil's content with the ref value if both test are succeed*/);

	/// Load package model
	Model ourModel("resources/objects/backpack/backpack.obj");

	Shader normalShader("vs.glsl", "fs.glsl", "gs.glsl");
	Shader modelShader("model-vs.glsl", "model-fs.glsl");
	Shader outlineShader("model-outline-vs.glsl", "model-outline-fs.glsl");

	// Get the uniform blocks
	unsigned int normalMatrices = glGetUniformBlockIndex(normalShader.ID, "Matrices");
	unsigned int modelMatrices = glGetUniformBlockIndex(modelShader.ID, "Matrices");
	unsigned int outlineMatrices = glGetUniformBlockIndex(outlineShader.ID, "Matrices");
	// Bind uniform blocks to the binding point 0
	glUniformBlockBinding(normalShader.ID, normalMatrices, 0 /*binding point idx*/);
	glUniformBlockBinding(modelShader.ID, modelMatrices, 0);
	glUniformBlockBinding(outlineShader.ID, outlineMatrices, 0);
	// Gen uniform buffer object
	unsigned int uboMatrices;
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// Bind uniform buffer object to the binding point 0
	glBindBufferRange(GL_UNIFORM_BUFFER, 0 /*binding point idx*/, uboMatrices,
		0 /*offset*/, 1 * sizeof(glm::mat4) /*size of data*/);

	// draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		Settings::deltaTime = currentFrame - Settings::lastFrame;
		Settings::lastFrame = currentFrame;

		Utils::processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 rotate = glm::rotate(model, glm::radians(45.0f), glm::vec3(cos(glfwGetTime()), sin(glfwGetTime()), cos(glfwGetTime())));
		glm::mat4 view = Settings::camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 1.0f, 100.0f);

		// Set values for the uniform buffer object
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0 /*view matrix offset*/,
			sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glStencilMask(0x00);
		//// Draw model normal
		normalShader.use();
		normalShader.setFloat("time", glfwGetTime());
		normalShader.setFloat("MAGNITUDE", glfwGetTime());
		normalShader.setMat4("model", rotate);
		normalShader.setMat4("projection", projection);
		ourModel.Draw(normalShader);

		// Draw main model
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // stencil testing always pass and replace stencil's content with 1s
		glStencilMask(0xFF);
		// Draw package model
		modelShader.use();
		modelShader.setMat4("model", rotate);
		modelShader.setMat4("projection", projection);
		ourModel.Draw(modelShader);

		// Draw outline for package model
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // only draw fragments that has stencil value not equal to 1
		glStencilMask(0x00); // disable writing to the stencil buffer
		// Draw scale model here
		outlineShader.use();
		glm::mat4 scale = glm::scale(rotate, glm::vec3(1.023f, 1.023, 1.023));
		outlineShader.setMat4("model", scale);
		outlineShader.setMat4("projection", projection);
		outlineShader.setFloat("time", glfwGetTime());
		ourModel.Draw(outlineShader);

		// Reset everything to default
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
