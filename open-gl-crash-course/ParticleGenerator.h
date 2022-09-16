#pragma once

#include"GameObject.h"
#include<vector>

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;
	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class ParticleGenerator
{
public:
	std::vector<Particle> Particles;
	ParticleGenerator(Shader& shader, Texture2D& texture);
	void Update(float dt, GameObject &gameObject, glm::vec2 offset);
	unsigned int GetUnusedParticle();
	void RespawnParticle(Particle& particle, GameObject& gameObject, glm::vec2 offset);
	void Draw();
private:
	Shader shader;
	Texture2D texture;
	unsigned int maxParticles = 500;
	unsigned int newParticlesPerFrame = 2;
	unsigned int lastUnusedIdx = 0;
	unsigned int particleVAO;
	void initParticles();
	void addParticles(GameObject& gameObject, glm::vec2 offset);
	void updateParticles(float dt);
};

