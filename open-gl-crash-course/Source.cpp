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
	// positions       
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f
};

float quadVertices[] = {
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f,  1.0f, 1.0f
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
	//glfwWindowHint(GLFW_SAMPLES, 4);

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
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	// Stencil testing
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP /*keep the stencil's content if stencil testing fail*/,
		GL_KEEP /*keep the stencil's content if depth testing fail*/,
		GL_REPLACE /*replace the stencil's content with the ref value if both test are succeed*/);

	unsigned int samples = 4;
	// MS Frambuffer
	unsigned int msFBO;
	glGenFramebuffers(1, &msFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
	// Gen the color buffer 
	unsigned int texColorbuffer;
	glGenTextures(1, &texColorbuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texColorbuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, 
		Settings::SCR_WIDTH, Settings::SCR_HEIGHT, GL_TRUE);
	// Bind texture buffer to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
		GL_TEXTURE_2D_MULTISAMPLE, texColorbuffer, 0);
	// Gen the render buffer
	unsigned int renderBuffer;
	glGenRenderbuffers(1, &renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, 
		Settings::SCR_WIDTH, Settings::SCR_HEIGHT);
	// Bind the render buffer object to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// Non MS Frambuffer
	unsigned int nmsFBO;
	glGenFramebuffers(1, &nmsFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, nmsFBO);
	// Gen the color buffer 
	unsigned int nmsTexColorbuffer;
	glGenTextures(1, &nmsTexColorbuffer);
	glBindTexture(GL_TEXTURE_2D, nmsTexColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Settings::SCR_WIDTH, Settings::SCR_HEIGHT, 0,
		GL_RGB, GL_UNSIGNED_BYTE, NULL /*we just alocate a memory for this texture - not fill it*/);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Bind texture buffer to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nmsTexColorbuffer, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Cube
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Quad
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	Shader aaShader("aa_vs.glsl", "aa_fs.glsl");
	Shader postShader("aa_post_vs.glsl", "aa_post_fs.glsl");
	postShader.use();
	postShader.setInt("screenTexture", 0);
	
	// draw as wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		Settings::deltaTime = currentFrame - Settings::lastFrame;
		Settings::lastFrame = currentFrame;

		Utils::processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = Settings::camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(Settings::camera.Zoom), (float)Settings::SCR_WIDTH / (float)Settings::SCR_HEIGHT, 1.0f, 100.0f);
		
		aaShader.use();
		aaShader.setMat4("model", model);
		aaShader.setMat4("view", view);
		aaShader.setMat4("projection", projection);
		
		// Draw with msFBO
		glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		// Blitting the multisampled image to the screen
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, nmsFBO);
		glBlitFramebuffer(0, 0, Settings::SCR_WIDTH, Settings::SCR_HEIGHT, 0, 0, 
			Settings::SCR_WIDTH, Settings::SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		// Draw post processing screen
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		postShader.use();
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, nmsTexColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
