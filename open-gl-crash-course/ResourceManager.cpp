#include "ResourceManager.h"

// Init static variables
std::unordered_map<std::string, Shader> ResourceManager::Shaders;
std::unordered_map<std::string, Texture2D> ResourceManager::Texture2Ds;

Shader& ResourceManager::LoadShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath, const std::string& name)
{
	if (Shaders.find(name) == Shaders.end()) {
		Shaders[name] = LoadShaderFromFile(vertexPath, fragmentPath, geometryPath);
	}
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
	if (Texture2Ds.find(name) == Texture2Ds.end()) {
		Texture2Ds[name] = LoadTextureFromFile(file, alpha);
	}
	return Texture2Ds[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
	return Texture2Ds[name];
}

std::vector<std::vector<unsigned int>> ResourceManager::LoadLevel(const char* file)
{
	std::string line;
	std::ifstream fstream(file);
	
	unsigned int tileCode;
	std::vector<std::vector<unsigned int>> tileData = std::vector<std::vector<unsigned int>>();
	
	if (!fstream.is_open())
	{
		std::cerr << "Could not open the file - '" << file << "'" << std::endl;
	}

	while (std::getline(fstream, line, '\n')) {
		std::istringstream sstream(line);
		std::vector<unsigned int> row;
		while (sstream >> tileCode) {
			row.push_back(tileCode);
		}
		tileData.push_back(row);
	}

	return tileData;
}

void ResourceManager::Clear()
{
	// Delete all shaders
	for (auto &shaderIter : Shaders) {
		glDeleteShader(shaderIter.second.ID);
	}
	// Delete all textures
	for (auto &textureIter : Texture2Ds) {
		glDeleteTextures(1, &textureIter.second.ID);
	}
}

Shader ResourceManager::LoadShaderFromFile(const char* vertexPath, const char* fragmentPath, 
	const char* geometryPath)
{
	// retrieve vertex and fragment source code
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer content into stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		
		if (geometryPath != nullptr) {
			std::ifstream gShaderFile;
			std::stringstream gShaderStream;
			
			gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			gShaderFile.open(geometryPath);
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	
	return Shader(vShaderCode, fShaderCode, geometryPath != nullptr ? gShaderCode : nullptr);
}

Texture2D ResourceManager::LoadTextureFromFile(const char* file, bool alpha)
{
	Texture2D texture;
	if (alpha) {
		texture.InternalFormat = GL_RGBA;
		texture.ImageFormat = GL_RGBA;
	}
	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
	if (data) {
		GLenum format = GL_RGB;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		texture.InternalFormat = format;
		texture.ImageFormat = format;
		texture.Generate(width, height, data);
	}
	else {
		std::cout << "====================TEXTURE==================" << std::endl;
		std::cout << "Width: " << width << std::endl;
		std::cout << "Height: " << height << std::endl;
		std::cout << "nrChannels: " << nrChannels << std::endl;
		std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
		std::cout << "====================TEXTURE==================" << std::endl;
	}

	stbi_image_free(data);
	return texture;
}
