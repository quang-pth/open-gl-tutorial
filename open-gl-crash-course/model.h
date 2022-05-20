#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"

class Model
{
public:
	Model(string const &path) {
		loadModel(path);
	}
	void Draw(Shader& shader) {
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}

private:
	// Model data
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	void loadModel(string const &path) {
		// Import model
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace /*post-processing options*/);
		// Check if model loaded successfully
		if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene) {
		// process all the node's meshes - if any
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			int meshIdx = node->mMeshes[i];
			// get the mesh stored in the scene object with corresponding meshIdx
			aiMesh *mesh = scene->mMeshes[meshIdx]; 
			// process and add the mesh to the model's meshes
			this->meshes.push_back(processMesh(mesh, scene));
		}
		// process other child nodes of the current processing node
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			aiNode* childNode = node->mChildren[i];
			processNode(childNode, scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		// Process mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			// Process vertex position attribute
			glm::vec3 vertexPos;
			vertexPos.x = mesh->mVertices[i].x;
			vertexPos.y = mesh->mVertices[i].y;
			vertexPos.z = mesh->mVertices[i].z;
			vertex.Position = vertexPos;
			// Process vertex normal attribute
			glm::vec3 vertexNormal;
			vertexNormal.x = mesh->mNormals[i].x;
			vertexNormal.y = mesh->mNormals[i].y;
			vertexNormal.z = mesh->mNormals[i].z;
			vertex.Normal = vertexNormal;
			// Process vertex texture coordinate attribute
			if (mesh->mTextureCoords[0] != NULL) {
				glm::vec2 textureCoords;
				textureCoords.x = mesh->mTextureCoords[0][i].x;
				textureCoords.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = textureCoords;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			// Add the processed vertex to our vertices list
			vertices.push_back(vertex);
		}
		// Process mesh's indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace meshFace = mesh->mFaces[i];
			for (unsigned int j = 0; j < meshFace.mNumIndices; j++) {
				indices.push_back(meshFace.mIndices[j]);
			}
		}
		// Process mesh's materials
		if (mesh->mMaterialIndex >= 0) {
			aiMaterial *meshMaterial = scene->mMaterials[mesh->mMaterialIndex];
			// Retrieve the diffuse maps
			vector<Texture> diffuseMaps = loadMaterialTextures(meshMaterial, aiTextureType_DIFFUSE, "diffuse_texture");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// Retrieve the specular maps
			vector<Texture> specularMaps = loadMaterialTextures(meshMaterial, aiTextureType_SPECULAR, "specular_texture");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// Retrieve the normal maps
			vector<Texture> normalMaps = loadMaterialTextures(meshMaterial, aiTextureType_NORMALS, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// Retrieve the specular maps
			vector<Texture> heightMaps = loadMaterialTextures(meshMaterial, aiTextureType_HEIGHT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString texturePath;
			// Get texture file location
			mat->GetTexture(type, i, &texturePath);
			bool skip = false;
			// Skip current loading texture if it's alreay loaded
			for (unsigned int j = 0; j < textures_loaded.size(); j++) {
				int isLoadedModel = strcmp(textures_loaded[j].path.data(), texturePath.C_Str());
				if (isLoadedModel == 0) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			// Load new texture if not loaded
			if (!skip) {
				Texture texture;
				texture.id = TextureFromFile(texturePath.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = texturePath.C_Str();
				texture.shininess = loadMaterialShininess(mat);
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}

	float loadMaterialShininess(aiMaterial* mat) {
		float shininess;
		if (AI_SUCCESS != aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess)) {
			// set default shininess if unsuccessful
			shininess = 32.0f;
		}
		return shininess;
	}

	unsigned int TextureFromFile(const char* path, const string& directory) {
		string fileName = string(path);
		fileName = directory + "/" + fileName;

		// Create Texture object
		unsigned int textureID;
		glGenTextures(1, &textureID);
		// Load and generate the Texture
		int width, height, nrChannels;

		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

		unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0); // get image width, height and color channels

		if (data) {
			GLenum format = GL_RGB;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0 /*Mipmap level*/,
				format /*format to store the image*/, width, height, 0,
				format /*format of the source image*/, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Set the texture wrapping parameters
			// S, T, R (3D texture) => x, y, z axis
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set mirror mode on texture x axis
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // set mirror mode on texture y axis
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

		return textureID;
	}
};

#endif
