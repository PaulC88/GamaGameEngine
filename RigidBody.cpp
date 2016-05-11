#include "Shape.h"
#include "RigidBody.h"

namespace GamaGameEngine {

	RigidBody::RigidBody() {
		//Empty
	}

	RigidBody::RigidBody(Shape* shape_, int x, int y)
		: shape(shape_->clone())
	{
		shape->m_body = this;
		m_position.set((float)x, (float)y);
		m_velocity.set(0, 0);
		m_angularVelocity = 0;
		m_torque = 0;
		m_orient = Random(-PI, PI);
		m_force.set(0, 0);
		m_staticFriction = 0.5f;
		m_dynamicFriction = 0.3f;
		m_restitution = 0.2f;
		shape->init();
		r = Random(0.2f, 1.0f);
		g = Random(0.2f, 1.0f);
		b = Random(0.2f, 1.0f);
	}

	void RigidBody::applyImpulse(const vec2& impulse, const vec2& contactVector) {
		m_velocity += m_inverseMass * impulse;
		m_angularVelocity += m_inverseInertia * cross(contactVector, impulse);
	}

	void RigidBody::setOrient(float radians) {
		m_orient = radians;
		shape->setOrient(radians);
	}

	void RigidBody::setStatic(void) {
		m_inertia = 0.0f;
		m_inverseInertia = 0.0f;
		m_mass = 0.0f;
		m_inverseMass = 0.0f;
	}

}