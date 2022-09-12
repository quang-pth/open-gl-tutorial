#pragma once

#include<unordered_map>
#include<string>
#include<fstream>
#include<sstream>
#include<glad/glad.h>
#include"Texture2D.h"
#include"Shader.h"
#include"stb_image.h"

class ResourceManager
{
public:
	static std::unordered_map<std::string, Shader> Shaders;
	static std::unordered_map<std::string, Texture2D> Texture2Ds;
	// Shader
	static Shader& LoadShader(const char* vertexPath, const char* fragmentPath,
		const char* geometryPath, const std::string &name);
	static Shader& GetShader(std::string name);
	// Texture
	static Texture2D& LoadTexture(const char* file, bool alpha, std::string name);
	static Texture2D& GetTexture(std::string name);
	// Game Level
	static std::vector<std::vector<unsigned int>> LoadLevel(const char* file);
	static void Clear();
private:
	// Implement Singleton pattern
	ResourceManager();
	static Shader LoadShaderFromFile(const char* vertexPath, const char* fragmentPath, 
		const char* geometryPath = nullptr);
	static Texture2D LoadTextureFromFile(const char* file, bool alpha);
};

