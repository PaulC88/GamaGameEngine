#pragma once

#include "Shape.h"

namespace GamaGameEngine {

	class Manifold;
	class RigidBody;

	typedef void(*CollisionCallback)(Manifold* m, RigidBody* a, RigidBody* b);

	extern CollisionCallback dispatch[Shape::COUNT][Shape::COUNT];

	void circletoCircle(Manifold* m, RigidBody* a, RigidBody* b);
	void circletoPolygon(Manifold* m, RigidBody* a, RigidBody* b);
	void polygontoCircle(Manifold* m, RigidBody* a, RigidBody* b);
	void polygontoPolygon(Manifold* m, RigidBody* a, RigidBody* b);

}