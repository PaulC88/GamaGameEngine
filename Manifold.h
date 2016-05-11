#pragma once

#include "GamaMath.h"

namespace GamaGameEngine {

	class RigidBody;

	class Manifold	{
	public:
		Manifold(RigidBody* a, RigidBody* b)
			: A(a)
			, B(b)
		{
		}

		void getCollisionInfo(void);
		void calculateCollisionProperties(void);
		void applyImpulse(void);
		void positionalCorrection(void);
		void infiniteMassCorrection(void);

		RigidBody* A;
		RigidBody* B;

		float penetration;
		float rest;
		float dynFrict;
		float statFrict;
		vec2 normal;
		vec2 contacts[2];
		unsigned int contactCount;
	};
}