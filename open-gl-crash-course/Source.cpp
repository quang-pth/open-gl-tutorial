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

	// Vertex Shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach vertex shader source to shader object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Check if vertex shader is rendered successfully
	checkComplieShader(vertexShader, "VERTEX");

	// Fragment Shader Object
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderOriginalSource, NULL);
	glCompileShader(fragmentShader);
	// Check if vertex shader is rendered successfully
	checkComplieShader(fragmentShader, "FRAGMENT");

	// Shader Program Object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	// Attach vertex and fragment shader to Shader program and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	checkComplieShader(shaderProgram, "PROGRAM");

	// Yellow Fragment Shader Object
	unsigned int yellowFragmentShader;
	yellowFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(yellowFragmentShader, 1, &yellowFragmentShaderSource, NULL);
	glCompileShader(yellowFragmentShader);
	// Check if vertex shader is rendered successfully
	checkComplieShader(yellowFragmentShader, "FRAGMENT");
	// Shader Program Object
	unsigned int yellowShaderProgram;
	yellowShaderProgram = glCreateProgram();
	// Attach vertex and fragment shader to Shader program and link them
	glAttachShader(yellowShaderProgram, vertexShader);
	glAttachShader(yellowShaderProgram, yellowFragmentShader);
	glLinkProgram(yellowShaderProgram);
	checkComplieShader(yellowShaderProgram, "PROGRAM");
	// Delete vertex and fragment shader after linked it to Shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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
	unsigned int VAO1, VBO1;
	// Create VAO to manage EBO, VBO and attributes pointers
	glGenVertexArrays(1, &VAO1);
	// Create Vertex Buffer Object (VBO) to manage Vertices Data
	glGenBuffers(1, &VBO1); // generate VBO's id
	glBindVertexArray(VAO1); // bind the Vertex Array Object
	
	// Bound VBO to the GL_ARRAY_BUFFER and bind the corresponding VBO and attributes pointers to VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	// Store Vertex Data within memory of the GPU as managed by VBO 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	
	// Configure the vertex attributes pointers on VBO
	glVertexAttribPointer(0, 3 /*attribute size*/, GL_FLOAT, GL_FALSE, 3 * sizeof(float) /*stride*/, (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex position attribute
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO after it's registered to VAO
	glBindVertexArray(0); // unbind the VAO for later uses
	// END BINDING CALLS

		// START BINDING CALLS
	unsigned int VAO2, VBO2;
	// Create VAO to manage EBO, VBO and attributes pointers
	glGenVertexArrays(1, &VAO2);
	// Create Vertex Buffer Object (VBO) to manage Vertices Data
	glGenBuffers(1, &VBO2); // generate VBO's id
	glBindVertexArray(VAO2); // bind the Vertex Array Object

	// Bound VBO to the GL_ARRAY_BUFFER and bind the corresponding VBO and attributes pointers to VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	// Store Vertex Data within memory of the GPU as managed by VBO 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// Configure the vertex attributes pointers on VBO
	glVertexAttribPointer(0, 3 /*attribute size*/, GL_FLOAT, GL_FALSE, 3 * sizeof(float) /*stride*/, (void*)0);
	glEnableVertexAttribArray(0); // enable the vertex position attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind VBO after it's registered to VAO
	glBindVertexArray(0); // unbind the VAO for later uses
	// END BINDING CALLS
	
	// Set drawing mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render loop
	while(!glfwWindowShouldClose(window)) {
		// Close GLFW when pressing Escape key
		processInput(window);

		// Rendering stuffs here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT); // state-using function

		// Draw original triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Use yellowShaderProgram to draw yellow triangle
		glUseProgram(yellowShaderProgram);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

	// Reset drawing mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// de-allocate resources once the program is about to exit
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VAO1);	
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VAO2);
	glDeleteProgram(shaderProgram);
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