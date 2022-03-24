#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include"Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
	
	// Init OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	// Build and Compile shader
	Shader ourShader("VertexShader.glsl", "FragmentShader.glsl");

	// Texture Coordinates Data (texture coordinates range from 0 to 1)
	float texCoords[] = {
		0.0f, 0.0f, // lower-left corner
		1.0f, 0.0f, // lower-right corner
		0.5f, 1.0f, // top-center corner
	};
	// Set OpenGL how to sampling the texture
	// S, T, R (3D texture) => x, y, z axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set mirror mode on texture x axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT); // set mirror mode on texture y axis
	// Set texture border color
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// Texture Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // nearst for scale downwards
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear for scale up
	// 7.2.1 mipmaps
	
	// Vertices Data
	float vertices[] = {
		// positions		// colors
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f  // top
	};

	// START BINDING CALLS
	unsigned int VAO, VBO;
	// Create VAO to manage EBO, VBO and attributes pointers
	glGenVertexArrays(1, &VAO);
	// Create Vertex Buffer Object (VBO) to manage Vertices Data
	glGenBuffers(1, &VBO); // generate VBO's id
	glBindVertexArray(VAO); // bind the Vertex Array Object
	
	// Bound VBO to the GL_ARRAY_BUFFER and bind the corresponding VBO and attributes pointers to VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Store Vertex Data within memory of the GPU as managed by VBO 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Configure the POSITION vertex attributes pointers on VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex position attribute
	// Configure the COLOR vertex attributes pointers on VBO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)) /*offset after the position attribute*/);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO after it's registered to VAO
	glBindVertexArray(0); // unbind the VAO for later uses
	// END BINDING CALLS
	
	// Render loop
	while(!glfwWindowShouldClose(window)) {
		// Close GLFW when pressing Escape key
		processInput(window);

		// Rendering stuffs here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT); // state-using function

		ourShader.use();
		// Draw a Triangle by using VAO
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	// de-allocate resources once the program is about to exit
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}