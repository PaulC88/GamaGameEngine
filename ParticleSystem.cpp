#include "ParticleSystem.h"
#include "ParticleBatch.h"
#include "SpriteBatch.h"

namespace GamaGameEngine {

	ParticleSystem::ParticleSystem() {
		//Empty
	}

	//delete particle batches
	ParticleSystem::~ParticleSystem() {
		for (auto& b : m_batches) {
			delete b;
		}
	}

	//Add particel batch
	void ParticleSystem::addParticleBatch(ParticleBatch* particleBatch) {
		m_batches.push_back(particleBatch);
	}

	//update particle batch
	void ParticleSystem::update(float deltaTime) {
		for (auto& b : m_batches) {
			b->update(deltaTime);
		}
	}

	//Draw particle batch
	void ParticleSystem::draw(SpriteBatch* spriteBatch) {
		for (auto& b : m_batches) {
			spriteBatch->begin();
			b->draw(spriteBatch);
			spriteBatch->end();
			spriteBatch->renderBatch();
		}
	}
}