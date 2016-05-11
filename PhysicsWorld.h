#pragma once

#include "RigidBody.h"
#include "Shape.h"
#include "Manifold.h"

#include  <vector>

namespace GamaGameEngine {

	class PhysicsWorld {

	public:
		PhysicsWorld(float dt, unsigned int iterations) :
			m_dt(dt),
			m_iterations(iterations) {}

		void step(void);
		void draw(void);
		GamaGameEngine::RigidBody* add(GamaGameEngine::Shape* shape, int x, int y);

		std::vector<GamaGameEngine::RigidBody*> bodies;
		std::vector<GamaGameEngine::Manifold> contacts;

	private:
		float m_dt;
		unsigned int m_iterations;
	};

}