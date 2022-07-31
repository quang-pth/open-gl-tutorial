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

float quadVertices[] = {
	// positions		// colors
	-0.05f,  0.05f,		1.0f, 0.0f, 0.0f, 
	 0.05f, -0.05f,		0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f,		0.0f, 0.0f, 1.0f,

	-0.05f,  0.05f,		1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,		0.0f, 1.0f, 0.0f,
	 0.05f,  0.05f,		0.0f, 1.0f, 1.0f,
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

	// Object offsets
	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2) {
		for (int x = -10; x < 10; x += 2) {
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index] = translation;
			index += 1;
		}
	}

	// Config vao, vbo
	unsigned int instancingVAO, instancingVBO, offsetInstanceVBO;
	glGenVertexArrays(1, &instancingVAO);
	glGenBuffers(1, &instancingVBO);
	glGenBuffers(1, &offsetInstanceVBO);
	// Bind vao
	glBindVertexArray(instancingVAO);
	// Position and color attributes
	glBindBuffer(GL_ARRAY_BUFFER, instancingVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Offset attribute with instanced array
	glBindBuffer(GL_ARRAY_BUFFER, offsetInstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2 /*attribute at location 2 is an instanced array*/, 
		1 /*update the content of the offset attribute for every instances*/);
	// Unbind vao
	glBindVertexArray(0);

	// Shader program
	Shader instancingShader("instancing_vs.glsl", "instancing_fs.glsl");

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
		glm::mat4 view = Settings::camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 1.0f, 100.0f);
		
		instancingShader.use();
		instancingShader.setMat4("model", model);
		instancingShader.setMat4("view", view);
		instancingShader.setMat4("projection", projection);

		glBindVertexArray(instancingVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

		// Reset everything to default
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &instancingVAO);
	glDeleteBuffers(1, &instancingVBO);
	glDeleteBuffers(1, &offsetInstanceVBO);

	glfwTerminate();
	return 0;
}
