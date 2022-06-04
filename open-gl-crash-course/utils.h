#ifndef UTIlS_H
#define UTIlS_H

#include"Shader.h"
#include"settings.h"
#include <GLFW/glfw3.h>
#include<vector>
#include<string>

#pragma once
class Utils
{
public:
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void processInput(GLFWwindow* window);
	static unsigned int loadTexture(const std::string& path, const GLenum& wrappingFormat = GL_LINEAR);
	static unsigned int loadCubeMap(const std::vector<std::string>& faces);
	static void setPointLight(const Shader& shader, const int& index, const glm::vec3& position, const glm::vec3& color);
};

#endif