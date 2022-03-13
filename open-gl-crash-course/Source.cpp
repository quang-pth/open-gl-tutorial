#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char* vertexShaderSource = "#version 330 core\n"
	"layout(location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
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

	// Render loop
	while(!glfwWindowShouldClose(window)) {
		// Close GLFW when pressing Escape key
		processInput(window);

		// Rendering stuffs here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // state-setting function
		glClear(GL_COLOR_BUFFER_BIT); // state-using function

		// Vertices Data
		float vertices[] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f,
		};
		// Create Vertex Buffer Object (VBO) to manage Vertices Data
		unsigned int VBO;
		glGenBuffers(1, &VBO); // generate VBO's id
		glBindBuffer(GL_ARRAY_BUFFER, VBO); // bound VBO object
		// Store Vertex Data within mermory of the GPU as managed by VBO 
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Vertex Shader object
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Attach vertex shader source to shader object
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// Check if vertex shader is rendered successfully
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR:SHADER:VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		glfwSwapBuffers(window);
		// Trigger keyboard input or mouse events => update window state
		glfwPollEvents();
	}

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
