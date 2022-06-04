#include "utils.h"
#include "stb_image.h"

void Utils::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Utils::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	
	if (Settings::firstMouse) {
		Settings::lastX = xpos;
		Settings::lastY = ypos;
		Settings::firstMouse = false;
	}

	float xoffset = xpos - Settings::lastX;
	float yoffset = Settings::lastY - ypos;
	Settings::lastX = xpos;
	Settings::lastY = ypos;

	Settings::camera.ProcessMouseMovement(xoffset, yoffset);
}

void Utils::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Settings::camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void Utils::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	/*
	=============================================
		START Control Camera
	=============================================
	*/
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Settings::camera.ProcessKeyboard(FORWARD, Settings::deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Settings::camera.ProcessKeyboard(BACKWARD, Settings::deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Settings::camera.ProcessKeyboard(LEFT, Settings::deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Settings::camera.ProcessKeyboard(RIGHT, Settings::deltaTime);
	}
	/*
	=============================================
		END Control Camera
	=============================================
	*/
}

void Utils::setPointLight(const Shader& shader, const int& index, const glm::vec3& position, const glm::vec3& color) {
	std::string prefix = "pointLight[" + std::to_string(index) + "]";
	// Set light point attributes
	shader.setVec3(prefix + ".position", position);
	shader.setVec3(prefix + ".ambient", color * 0.1f);
	shader.setVec3(prefix + ".diffuse", color * 1.3232f);
	shader.setVec3(prefix + ".specular", color * 0.08f);
	shader.setFloat(prefix + ".constant", 1.0f);
	shader.setFloat(prefix + ".linear", 0.07f);
	shader.setFloat(prefix + ".quadratic", 0.017f);
}

unsigned int Utils::loadTexture(const std::string& path, const GLenum& wrappingFormat) {
	// Create Texture object
	unsigned int textureID;
	glGenTextures(1, &textureID);
	// Load and generate the Texture
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0); // get image width, height and color channels

	if (data) {
		// Set texture color channels
		GLenum colorChannel = GL_RGB;
		if (nrChannels == 1)
			colorChannel = GL_RED;
		else if (nrChannels == 3)
			colorChannel = GL_RGB;
		else if (nrChannels == 4)
			colorChannel = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0 /*Mipmap level*/,
			colorChannel /*format to store the image*/, width, height, 0,
			colorChannel /*format of the source image*/, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the texture wrapping parameters
		// S, T, R (3D texture) => x, y, z axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappingFormat); // set mirror mode on texture x axis
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappingFormat); // set mirror mode on texture y axis
		// Set Texture Filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmap filtering for texture get scaled down
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linear for scale up
	}
	else {
		std::cout << "====================TEXTURE==================" << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "nrChannels: " << nrChannels << std::endl;
		std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
		std::cout << "====================TEXTURE==================" << std::endl;
	}
	// Free the image memory
	stbi_image_free(data);
	// Reset
	stbi_set_flip_vertically_on_load(false);

	return textureID;
}

unsigned int Utils::loadCubeMap(const std::vector<std::string>& faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data != NULL) {
			stbi_set_flip_vertically_on_load(false); // cubemap origin is top-left corner => not flip
			// Gen texture for each face of the cube (6 faces)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			std::cout << "Cubemap failed to load at path: " << faces[i].c_str() << std::endl;
		}

		stbi_image_free(data);
	}
	// set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}