#include "Manifold.h"
#include "Collision.h"
#include "RigidBody.h"
#include "Shape.h"

namespace GamaGameEngine {

	void Manifold::getCollisionInfo(void) {
		dispatch[A->shape->getType()][B->shape->getType()](this, A, B);
	}

	void Manifold::calculateCollisionProperties(void)	{
		rest = std::min(A->getRestitution(), B->getRestitution());

		statFrict = std::sqrt(A->getStatFriction() * B->getStatFriction());
		dynFrict = std::sqrt(A->getDynFriction() * B->getDynFriction());

		for (unsigned int i = 0; i < contactCount; ++i) {
			vec2 distA = contacts[i] - A->getPosition();
			vec2 distB = contacts[i] - B->getPosition();

			vec2 relVelocity = B->getVelocity() + cross(B->getAngularVelocity(), distB) -
				A->getVelocity() - cross(A->getAngularVelocity(), distA);

			if (relVelocity.lengthSquared() < (dt * gravity).lengthSquared() + EPSILON)
				rest = 0.0f;
		}
	}

	void Manifold::applyImpulse(void) {
		if (equal(A->getInvMass() + B->getInvMass(), 0)) {
			infiniteMassCorrection();
			return;
		}

		for (unsigned int i = 0; i < contactCount; ++i) {
			vec2 distA = contacts[i] - A->getPosition();
			vec2 distB = contacts[i] - B->getPosition();

			vec2 relVelocity = B->getVelocity() + cross(B->getAngularVelocity(), distB) -
				A->getVelocity() - cross(A->getAngularVelocity(), distA);

			float contactVel = dot(relVelocity, normal);

			if (contactVel > 0)
				return;

			float raCrossN = cross(distA, normal);
			float rbCrossN = cross(distB, normal);
			float invMassSum = A->getInvMass() + B->getInvMass() + Sqr(raCrossN) * A->getInvInertia() + Sqr(rbCrossN) * B->getInvInertia();

			float j = -(1.0f + rest) * contactVel;
			j /= invMassSum;
			j /= (float)contactCount;

			vec2 impulse = normal * j;
			A->applyImpulse(-impulse, distA);
			B->applyImpulse(impulse, distB);

			relVelocity = B->getVelocity() + cross(B->getAngularVelocity(), distB) -
				A->getVelocity() - cross(A->getAngularVelocity(), distA);

			vec2 t = relVelocity - (normal * dot(relVelocity, normal));
			t.normalise();

			float jt = -dot(relVelocity, t);
			jt /= invMassSum;
			jt /= (float)contactCount;

			if (equal(jt, 0.0f))
				return;

			// Coulumb's law
			vec2 tangentImpulse;
			if (std::abs(jt) < j * statFrict)
				tangentImpulse = t * jt;
			else
				tangentImpulse = t * -j * dynFrict;

			A->applyImpulse(-tangentImpulse, distA);
			B->applyImpulse(tangentImpulse, distB);
		}
	}

	void Manifold::positionalCorrection(void) {

		const float k_slop = 0.05f;
		const float percent = 0.4f; 
		vec2 correction = (std::max(penetration - k_slop, 0.0f) / (A->getInvMass() + B->getInvMass())) * normal * percent;

		A->setPosition(A->getPosition() - correction * A->getInvMass());
		B->setPosition(B->getPosition() + correction * B->getInvMass());
	}

	void Manifold::infiniteMassCorrection(void)	{
		A->setVelocity(vec2(0, 0));
		B->setVelocity(vec2(0, 0));
	}
}