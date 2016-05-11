#include "PhysicsWorld.h"
#include <GL/glut.h>
#include <iostream>

namespace GamaGameEngine {

	void integrateForces(GamaGameEngine::RigidBody* b, float dt) {
		if (b->getInvMass() == 0.0f)
			return;

		b->setVelocity(b->getVelocity() + (b->getForce() * b->getInvMass() + GamaGameEngine::gravity) * (dt / 2.0f));
		b->setAngularVelocity(b->getAngularVelocity() + b->getTorque() * b->getInvInertia() * (dt / 2.0f));
	}

	void integrateVelocity(GamaGameEngine::RigidBody* b, float dt)	{
		if (b->getInvMass() == 0.0f)
			return;

		b->setPosition(b->getPosition() + b->getVelocity() * dt);
		b->setOrient(b->getOrient() + b->getAngularVelocity() * dt);
		b->setOrient(b->getOrient());
		integrateForces(b, dt);
	}

	void PhysicsWorld::step(void) {

		contacts.clear();
		for (unsigned int i = 0; i < bodies.size(); ++i) {

			GamaGameEngine::RigidBody* A = bodies[i];
			for (unsigned int j = i + 1; j < bodies.size(); ++j) {

				GamaGameEngine::RigidBody* B = bodies[j];
				if ((A->getInvMass() == 0 && B->getInvMass() == 0) 
					|| (A->getIgnoreCase() && B->getIgnoreCase())
					|| (A->getEmpty() || B->getEmpty()))
					continue;

				GamaGameEngine::Manifold m(A, B);
				m.getCollisionInfo();
				if (m.contactCount)
					contacts.emplace_back(m);
			}
		}

		// Integrate forces
		for (unsigned int i = 0; i < bodies.size(); ++i)
			integrateForces(bodies[i], m_dt);

		// Initialize collision
		for (unsigned int i = 0; i < contacts.size(); ++i)
			contacts[i].calculateCollisionProperties();

		// Solve collisions
		for (unsigned int j = 0; j < m_iterations; ++j)
			for (unsigned int i = 0; i < contacts.size(); ++i)
				contacts[i].applyImpulse();

		// Integrate velocities
		for (unsigned int i = 0; i < bodies.size(); ++i)
			integrateVelocity(bodies[i], m_dt);

		// Correct positions
		for (unsigned int i = 0; i < contacts.size(); ++i)
			contacts[i].positionalCorrection();

		// Clear all forces
		for (unsigned int i = 0; i < bodies.size(); ++i) {
			GamaGameEngine::RigidBody* b = bodies[i];
			b->setForce(vec2(0, 0));
			b->setTorque(0.0f);
		}

	}

	void PhysicsWorld::draw(void) {

		for (unsigned int i = 0; i < bodies.size(); ++i) {
			GamaGameEngine::RigidBody* b = bodies[i];
			b->shape->draw();
		}
	}

	GamaGameEngine::RigidBody* PhysicsWorld::add(GamaGameEngine::Shape* shape, int x, int y) {

		assert(shape);
		GamaGameEngine::RigidBody* b = new GamaGameEngine::RigidBody(shape, x, y);
		bodies.push_back(b);
		return b;
	}
}