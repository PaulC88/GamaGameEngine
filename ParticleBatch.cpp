#include "ParticleBatch.h"

namespace GamaGameEngine {

	void Particle::update(float deltaTime) {
		m_position += m_velocity * deltaTime;
	}

	ParticleBatch::ParticleBatch() {
		//Empty
	}

	//delete particles
	ParticleBatch::~ParticleBatch() {
		delete[] m_particles;
	}

	//Initialise particle batch
	void ParticleBatch::init(int maxParticles, float decayRate, GLTexture texture) {
		m_maxParticles = maxParticles;
		m_particles = new Particle[maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;
	}

	//Update particle batch
	void ParticleBatch::update(float deltaTime) {
		for (int i = 0; i < m_maxParticles; i++) {
			if (m_particles[i].m_life > 0.0f) {
				m_particles[i].update(deltaTime);
				m_particles[i].m_life -= m_decayRate * deltaTime;
			}
		}
	}

	//Draw particle batch
	void ParticleBatch::draw(SpriteBatch* spriteBatch) {
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++) {
			auto& p = m_particles[i];
			if (p.m_life > 0.0f) {
				glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);
				spriteBatch->addSprite(destRect, uvRect, m_texture.id, 0.0f, p.m_colour);
			}
		}
	}

	//Add particle to batch
	void ParticleBatch::addParticle(const glm::vec2& position, const glm::vec2& velocity, const Colour& colour, float width) {
		
		int paticleIndex = findFreePartcle();

		auto& p = m_particles[paticleIndex];

		p.m_life = 1.0f;
		p.m_position = position;
		p.m_velocity = velocity;
		p.m_colour = colour;
		p.m_width = width;
	}

	//find next free particle
	int ParticleBatch::findFreePartcle() {

		for (int i = m_lastFreeParticle; i < m_maxParticles; i++) {
			if (m_particles[i].m_life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		for (int i = 0; i < m_lastFreeParticle; i++) {
			if (m_particles[i].m_life <= 0.0f) {
				m_lastFreeParticle = i;
				return i;
			}
		}

		return 0;
	}
}