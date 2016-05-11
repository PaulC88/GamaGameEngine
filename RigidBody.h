#pragma once

#include "GamaMath.h"

namespace GamaGameEngine {

	class Shape;

	class RigidBody {

	public:
		RigidBody();

		RigidBody(Shape* shape_, int x, int y);

		void applyForce(const vec2& force) { m_force += force; }

		void applyImpulse(const vec2& impulse, const vec2& contactVector);
		
		//Getters
		vec2 getPosition() const { return m_position; }
		vec2 getVelocity() const { return m_velocity; }
		vec2 getForce() const { return m_force; }
		float getAngularVelocity() const { return m_angularVelocity; }
		float getTorque() const { return m_torque; }
		float getOrient() const { return m_orient; }
		float getMass() const { return m_mass; }
		float getInvMass() const { return m_inverseMass; }
		float getInertia() const { return m_inertia; }
		float getInvInertia() const { return m_inverseInertia; }
		float getStatFriction() const { return m_staticFriction; }
		float getDynFriction() const { return m_dynamicFriction; }
		float getRestitution() const { return m_restitution; }
		bool getIgnoreCase() const { return m_ignoreCase; }
		bool getEmpty() const { return m_empty; }

		//Setters
		void setVelocity(vec2 velocity) { m_velocity = velocity; }
		void setForce(vec2 force) { m_force = force; }
		void setPosition(vec2 position) { m_position = position; }
		void setYPosition(float posY) { m_position.y = posY; }
		void setXPosition(float posX) { m_position.x = posX; }
		void setAngularVelocity(float angVel) { m_angularVelocity = angVel; }
		void setTorque(float torque) { m_torque = torque; }
		void setMass(float mass) { m_mass = mass; }
		void setInvMass(float invMass) { m_inverseMass = invMass; }
		void setInertia(float inertia) { m_inertia = inertia; }
		void setInvInertia(float invInertia) { m_inverseInertia = invInertia; }
		void setRestitution(float restitution) { m_restitution = restitution; }
		void setStatFriction(float statFriction) { m_staticFriction = statFriction; }
		void setDynFriction(float dynFriction) { m_dynamicFriction = dynFriction; }
		void setIgnoreCase() { m_ignoreCase = true; }
		void setEmpty() { m_empty = true; }

		void setOrient(float radians);
		void setStatic(void);

		float r, g, b;

		Shape* shape;

		
	private:

		bool m_empty = false;
		bool m_ignoreCase = false;
		vec2 m_force, m_velocity, m_position;
		float m_angularVelocity, m_torque, m_orient;
		float m_inverseMass, m_inverseInertia, m_mass, m_inertia;
		float m_staticFriction, m_dynamicFriction, m_restitution;
	};

}