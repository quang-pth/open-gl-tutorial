#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader& shader, Texture2D& texture) : Particles(), particleVAO()
{
	this->shader = &shader;
	this->texture = &texture;
	this->initParticles();
}

void ParticleGenerator::addParticles(GameObject &gameObject, glm::vec2 offset)
{
	for (unsigned int i = 0; i < this->newParticlesPerFrame; i++) {
		unsigned int unusedParticleIdx = this->GetUnusedParticle();
		RespawnParticle(this->Particles[unusedParticleIdx], gameObject, offset);
	}
}

void ParticleGenerator::updateParticles(float dt)
{
	for (unsigned int i = 0; i < this->maxParticles; i++) {
		Particle& p = this->Particles[i];
		p.Life -= dt;
		if (p.Life >= 0) {
			p.Position -= p.Velocity * dt;
			p.Color.a -= dt * 2.0f;
		}
	}
}

void ParticleGenerator::initParticles()
{
	unsigned int VBO;
	float particle_quad[] = {
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, &this->particleVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(this->particleVAO);
	// fill mesh buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
	// set mesh attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	for (unsigned int i = 0; i < this->maxParticles; i++) {
		this->Particles.push_back(Particle());
	}
}

void ParticleGenerator::Update(float dt, GameObject &gameObject, glm::vec2 offset)
{
	this->addParticles(gameObject, offset);
	this->updateParticles(dt);
}

// Return the lastest unused particle index
unsigned int ParticleGenerator::GetUnusedParticle()
{
	// Search from the last unused particle index
	for (unsigned int i = this->lastUnusedIdx; i < this->maxParticles; i++) {
		if (this->Particles[i].Life <= 0.0f) {
			this->lastUnusedIdx = i;
			return i;
		}
	}
	// Linear search
	for (unsigned int i = 0; i < this->lastUnusedIdx; i++) {
		if (this->Particles[i].Life <= 0.0f) {
			this->lastUnusedIdx = i;
			return i;
		}
	}

	this->lastUnusedIdx = 0;
	return 0;
}

void ParticleGenerator::RespawnParticle(Particle& particle, GameObject& gameObject, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = gameObject.Position + random + offset;
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = gameObject.Velocity * 0.5f;
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader->Use();
	for (Particle& particle : this->Particles) {
		if (particle.Life > 0.0f) {
			this->shader->setVec2("offset", particle.Position);
			this->shader->setVec4("color", particle.Color);
			this->texture->Bind();
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(this->particleVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
