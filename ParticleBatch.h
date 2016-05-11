#pragma once

#include <glm/glm.hpp>

#include "SpriteBatch.h"
#include "DataTypes.h"

namespace GamaGameEngine {

	class Particle {

	public:
		friend class ParticleBatch;

		void update(float deltaTime);

	private:
		glm::vec2 m_position = glm::vec2(0.0f);
		glm::vec2 m_velocity = glm::vec2(0.0f);
		Colour m_colour;
		float m_life = 0.0f;
		float m_width = 0.0f;
	};

	class ParticleBatch	{
	
	public:
		ParticleBatch();
		~ParticleBatch();

		void init(int maxParticles, float decayRate, GLTexture texture);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

		void addParticle(const glm::vec2& position, const glm::vec2& velocity, const Colour& colour, float width);

	private:
		int findFreePartcle();

		float m_decayRate = 0.1f;
		Particle* m_particles = nullptr;
		int m_maxParticles = 0;
		int m_lastFreeParticle = 0;
		GLTexture m_texture;
	};
}