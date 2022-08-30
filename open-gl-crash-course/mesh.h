#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	// bone indexes
	int m_boneIDs[MAX_BONE_INFLUENCE];
	int w_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	string type;
	// path of texture
	string path;
	float shininess;
};

class Mesh
{
public:
	// Mesh data
	vector<Vertex> vertices;
	vector<unsigned int> indices; // order to draw a mesh by its vertices
	vector<Texture> textures;

	// Method definition
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		this->setUpMesh();
	}

	void Draw(Shader& shader) {
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i); // activate texture unit
			// Retrieve texture number (the N in diffuse_textureN)
			string number;
			string name = textures[i].type;

			if (name == "texture_diffuse") {
				number = to_string(diffuseNr++);
			}
			else if (name == "texture_specular") {
				number = to_string(specularNr++);
			}
			else if (name == "texture_normal") {
				number = to_string(normalNr++);
			}
			
			shader.setFloat((name + number).c_str(), i);
			//shader.setFloat((number).c_str(), textures[i].shininess);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}

		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		// set back to default
		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int VAO, VBO, EBO;

	// Method definition
	void setUpMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) /*size*/, &vertices[0] /*data*/, GL_STATIC_DRAW);
		// EBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) /*stride*/, (void*)0);
		// vertex normal attribute
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal) /*return the byte offset of Normal from the start of the struct*/);
		// vertex texcoords attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords) /*return the byte offset of TexCoords from the start of the struct*/);
		// vertex tangent attribute
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent attribute
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		// vertex m_boneIDs attribute
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_boneIDs));
		// vertex m_Wieghts attribute
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, w_Weights));

		glBindVertexArray(0);
	}
};

#endif