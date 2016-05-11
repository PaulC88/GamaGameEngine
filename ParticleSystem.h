#pragma once

#include <vector>

namespace GamaGameEngine {

	class ParticleBatch;
	class SpriteBatch;

	class ParticleSystem {

	public:
		ParticleSystem();
		~ParticleSystem();

		void addParticleBatch(ParticleBatch* particleBatch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		std::vector<ParticleBatch*> m_batches;
	};

}