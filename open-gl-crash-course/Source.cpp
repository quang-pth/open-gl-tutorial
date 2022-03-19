#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void checkComplieShader(GLuint shader, std::string shaderType);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"}\0"
;
const char* fragmentShaderOriginalSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0"
;
const char* yellowFragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\0"
;

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
	
	unsigned int vertexShader;
	unsigned int orangeFragmentShader;
	unsigned int yellowFragmentShader;
	unsigned int orangeShaderProgram;
	unsigned int yellowShaderProgram;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	orangeShaderProgram = glCreateProgram();
	yellowShaderProgram = glCreateProgram();

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	checkComplieShader(vertexShader, "VERTEX");

	glShaderSource(orangeFragmentShader, 1, &fragmentShaderOriginalSource, NULL);
	glCompileShader(orangeFragmentShader);
	checkComplieShader(orangeFragmentShader, "FRAGMENT");

	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
	glCompileShader(yellowFragmentShader);
	checkComplieShader(yellowFragmentShader, "FRAGMENT");

	// ORANGE Shader Program Object
	glAttachShader(orangeShaderProgram, vertexShader);
	glAttachShader(orangeShaderProgram, orangeFragmentShader);
	glLinkProgram(orangeShaderProgram);
	checkComplieShader(orangeShaderProgram, "PROGRAM");

	// YELLOW Shader Program Object
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);
	checkComplieShader(yellowShaderProgram, "PROGRAM");
	
	// Delete vertex and fragment shaders after linked it to shader program
	glDeleteShader(vertexShader);
	glDeleteShader(orangeFragmentShader);
	glDeleteShader(yellowFragmentShader);

	// Vertices Data
	float vertices1[] = {
		// First Triangle
		-0.5f, 0.5f, 0.0f, // top right
		-0.5f, 0.0f, 0.0f, // top left
		.0f, .0f, 0.0f, // bottom left
	};

	float vertices2[] = {
		// Second Triangle
		0.5f, 0.5f, 0.0f, // top right
		0.5f, .0f, 0.0f, // bottom right
		.0f, .0f, 0.0f, // bottom left
	};

	// START BINDING CALLS
	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glBindVertexArray(VAOs[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3 /*attribute size*/, GL_FLOAT, GL_FALSE, 3 * sizeof(float) /*stride*/, (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex position attribute
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO after it's registered to VAO
	glBindVertexArray(0); // unbind the VAO for later uses
	// END BINDING CALLS

	// START BINDING CALLS
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		// Draw original triangle
		glUseProgram(orangeShaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Use yellowShaderProgram to draw yellow triangle
		glUseProgram(yellowShaderProgram);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	// de-allocate resources once the program is about to exit
	glDeleteBuffers(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(orangeShaderProgram);
	glDeleteProgram(yellowShaderProgram);

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


void checkComplieShader(GLuint shader, std::string shaderType) {
	int success;
	char infoLog[512];
	if (shaderType != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
}