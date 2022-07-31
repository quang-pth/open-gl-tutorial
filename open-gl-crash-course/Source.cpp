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
	
	unsigned int amount = 5000;
	glm::mat4* modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	float radius = 50.0f;
	float offset = 5.0f;
	for (unsigned int i = 0; i < amount; i++) {
		glm::mat4 model = glm::mat4(1.0);
		// translate
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		// scale
		float scale = (rand() % 20) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));
		// rotate
		float rotateAngle = (rand() % 360);
		model = glm::rotate(model, rotateAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		modelMatrices[i] = model;
	}

	// Shader program
	Shader shader("planet_vs.glsl", "planet_fs.glsl");
	Shader asteriodShader("instancing_vs.glsl", "instancing_fs.glsl");
	Model planet("resources/objects/planet/mars/planet.obj");
	Model asteriod("resources/objects/planet/rock/rock.obj");

	// Config the asteriod instace matrix
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < asteriod.meshes.size(); i++) {
		unsigned int asteriodVAO = asteriod.meshes[i].VAO;
		glBindVertexArray(asteriodVAO);
		std::size_t vec4AttributeSize = sizeof(glm::vec4);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4AttributeSize, (void*)0);
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4AttributeSize, (void*)(1 * vec4AttributeSize));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * vec4AttributeSize, (void*)(2 * vec4AttributeSize));
		glEnableVertexAttribArray(9);
		glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 4 * vec4AttributeSize, (void*)(3 * vec4AttributeSize));
		glEnableVertexAttribArray(10);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);
		glVertexAttribDivisor(9, 1);
		glVertexAttribDivisor(10, 1);
		glBindVertexArray(0);
	}

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
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, currentFrame, glm::vec3(0, 1, 0));
		glm::mat4 view = Settings::camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 1.0f, 100.0f);
		
		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		// Draw planet
		planet.Draw(shader);
		// Draw asteriods with instanced array
		asteriodShader.use();
		asteriodShader.setMat4("view", view);
		asteriodShader.setMat4("projection", projection);
		for (unsigned int i = 0; i < asteriod.meshes.size(); i++) {
			glBindVertexArray(asteriod.meshes[i].VAO);
			glDrawElementsInstanced(GL_TRIANGLES, asteriod.meshes[i].indices.size(),
				GL_UNSIGNED_INT, 0, amount);
		}

		// Reset everything to default
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	glDeleteBuffers(1, &instanceVBO);

	glfwTerminate();
	return 0;
}
