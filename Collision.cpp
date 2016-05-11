#include "Collision.h"
#include "GamaMath.h"
#include "Manifold.h"
#include "RigidBody.h"


namespace GamaGameEngine {

	CollisionCallback dispatch[Shape::COUNT][Shape::COUNT] =	{
		{
			circletoCircle, circletoPolygon
		},
		{
			polygontoCircle, polygontoPolygon
		},
	};

	void circletoCircle(Manifold* m, RigidBody* a, RigidBody* b) {
		Circle* A = reinterpret_cast<Circle *>(a->shape);
		Circle* B = reinterpret_cast<Circle *>(b->shape);

		vec2 normal = b->getPosition() - a->getPosition();

		float distSqr = normal.lengthSquared();
		float radius = A->m_radius + B->m_radius;

		if (distSqr >= radius * radius) {
			m->contactCount = 0;
			return;
		}

		float distance = std::sqrt(distSqr);

		m->contactCount = 1;

		if (distance == 0.0f) {
			m->penetration = A->m_radius;
			m->normal = vec2(1, 0);
			m->contacts[0] = a->getPosition();
		}
		else {
			m->penetration = radius - distance;
			m->normal = normal / distance;
			m->contacts[0] = m->normal * A->m_radius + a->getPosition();
		}
	}

	void circletoPolygon(Manifold* m, RigidBody* a, RigidBody* b) {
		Circle* A = reinterpret_cast<Circle *>      (a->shape);
		Polygon* B = reinterpret_cast<Polygon *>(b->shape);

		m->contactCount = 0;

		vec2 center = a->getPosition();
		center = B->m_orientation.transpose() * (center - b->getPosition());

		float separation = -FLT_MAX;
		int faceNormal = 0;
		for (int i = 0; i < B->m_vertexCount; ++i) {
			float s = dot(B->m_normals[i], center - B->m_vertices[i]);

			if (s > A->m_radius)
				return;

			if (s > separation) {
				separation = s;
				faceNormal = i;
			}
		}

		vec2 v1 = B->m_vertices[faceNormal];
		unsigned int i2 = faceNormal + 1 < B->m_vertexCount ? faceNormal + 1 : 0;
		vec2 v2 = B->m_vertices[i2];

		if (separation < EPSILON) {
			m->contactCount = 1;
			m->normal = -(B->m_orientation * B->m_normals[faceNormal]);
			m->contacts[0] = m->normal * A->m_radius + a->getPosition();
			m->penetration = A->m_radius;
			return;
		}

		float dot1 = dot(center - v1, v2 - v1);
		float dot2 = dot(center - v2, v1 - v2);
		m->penetration = A->m_radius - separation;

		if (dot1 <= 0.0f) {
			if (DistSqr(center, v1) > A->m_radius * A->m_radius)
				return;

			m->contactCount = 1;
			vec2 n = v1 - center;
			n = B->m_orientation * n;
			n.normalise();
			m->normal = n;
			v1 = B->m_orientation * v1 + b->getPosition();
			m->contacts[0] = v1;
		}

		else if (dot2 <= 0.0f) {
			if (DistSqr(center, v2) > A->m_radius * A->m_radius)
				return;

			m->contactCount = 1;
			vec2 n = v2 - center;
			v2 = B->m_orientation * v2 + b->getPosition();
			m->contacts[0] = v2;
			n = B->m_orientation * n;
			n.normalise();
			m->normal = n;
		}

		else {
			vec2 n = B->m_normals[faceNormal];
			if (dot(center - v1, n) > A->m_radius)
				return;

			n = B->m_orientation * n;
			m->normal = -n;
			m->contacts[0] = m->normal * A->m_radius + a->getPosition();
			m->contactCount = 1;
		}
	}

	void polygontoCircle(Manifold* m, RigidBody* a, RigidBody* b) {
		circletoPolygon(m, b, a);
		m->normal = -m->normal;
	}

	float findAxisLeastPenetration(unsigned int* faceIndex, Polygon* A, Polygon* B)	{
		float bestDistance = -FLT_MAX;
		unsigned int bestIndex;

		for (int i = 0; i < A->m_vertexCount; ++i) {
			vec2 n = A->m_normals[i];
			vec2 nw = A->m_orientation * n;

			mat2 buT = B->m_orientation.transpose();
			n = buT * nw;

			vec2 s = B->getSupport(-n);

			vec2 v = A->m_vertices[i];
			v = A->m_orientation * v + A->m_body->getPosition();
			v -= B->m_body->getPosition();
			v = buT * v;

			float d = dot(n, s - v);

			if (d > bestDistance) {
				bestDistance = d;
				bestIndex = i;
			}
		}

		*faceIndex = bestIndex;
		return bestDistance;
	}

	void findIncidentFace(vec2* v, Polygon* RefPoly, Polygon* IncPoly, unsigned int referenceIndex) {
		vec2 referenceNormal = RefPoly->m_normals[referenceIndex];

		referenceNormal = RefPoly->m_orientation * referenceNormal; 
		referenceNormal = IncPoly->m_orientation.transpose() * referenceNormal; 

																
		unsigned int incidentFace = 0;
		float minDot = FLT_MAX;
		for (int i = 0; i < IncPoly->m_vertexCount; ++i)	{
			float dotProd = dot(referenceNormal, IncPoly->m_normals[i]);
			if (dotProd < minDot) {
				minDot = dotProd;
				incidentFace = i;
			}
		}

		v[0] = IncPoly->m_orientation * IncPoly->m_vertices[incidentFace] + IncPoly->m_body->getPosition();
		incidentFace = incidentFace + 1 >= (unsigned int)IncPoly->m_vertexCount ? 0 : incidentFace + 1;
		v[1] = IncPoly->m_orientation * IncPoly->m_vertices[incidentFace] + IncPoly->m_body->getPosition();
	}

	unsigned int clip(vec2 n, float c, vec2* face)	{
		unsigned int sp = 0;
		vec2 out[2] = {
			face[0],
			face[1]
		};

		float d1 = dot(n, face[0]) - c;
		float d2 = dot(n, face[1]) - c;

		if (d1 <= 0.0f) out[sp++] = face[0];
		if (d2 <= 0.0f) out[sp++] = face[1];

		if (d1 * d2 < 0.0f) {
			float alpha = d1 / (d1 - d2);
			out[sp] = face[0] + alpha * (face[1] - face[0]);
			++sp;
		}

		face[0] = out[0];
		face[1] = out[1];

		assert(sp != 3);

		return sp;
	}

	void polygontoPolygon(Manifold* m, RigidBody* a, RigidBody* b) {
		Polygon* A = reinterpret_cast<Polygon *>(a->shape);
		Polygon* B = reinterpret_cast<Polygon *>(b->shape);
		m->contactCount = 0;

		unsigned int faceA;
		float penetrationA = findAxisLeastPenetration(&faceA, A, B);
		if (penetrationA >= 0.0f)
			return;

		unsigned int faceB;
		float penetrationB = findAxisLeastPenetration(&faceB, B, A);
		if (penetrationB >= 0.0f)
			return;

		unsigned int referenceIndex;
		bool flip; 

		Polygon* RefPoly; 
		Polygon* IncPoly; 

		if (BiasGreaterThan(penetrationA, penetrationB)) {
			RefPoly = A;
			IncPoly = B;
			referenceIndex = faceA;
			flip = false;
		} 
		else {
			RefPoly = B;
			IncPoly = A;
			referenceIndex = faceB;
			flip = true;
		}

		vec2 incidentFace[2];
		findIncidentFace(incidentFace, RefPoly, IncPoly, referenceIndex);

		vec2 v1 = RefPoly->m_vertices[referenceIndex];
		referenceIndex = referenceIndex + 1 == RefPoly->m_vertexCount ? 0 : referenceIndex + 1;
		vec2 v2 = RefPoly->m_vertices[referenceIndex];

		v1 = RefPoly->m_orientation * v1 + RefPoly->m_body->getPosition();
		v2 = RefPoly->m_orientation * v2 + RefPoly->m_body->getPosition();

		vec2 sidePlaneNormal = (v2 - v1);
		sidePlaneNormal.normalise();

		vec2 refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

		float refC = dot(refFaceNormal, v1);
		float negSide = -dot(sidePlaneNormal, v1);
		float posSide = dot(sidePlaneNormal, v2);

		if (clip(-sidePlaneNormal, negSide, incidentFace) < 2)
			return;

		if (clip(sidePlaneNormal, posSide, incidentFace) < 2)
			return;

		m->normal = flip ? -refFaceNormal : refFaceNormal;

		unsigned int cp = 0; 
		float separation = dot(refFaceNormal, incidentFace[0]) - refC;
		if (separation <= 0.0f)	{
			m->contacts[cp] = incidentFace[0];
			m->penetration = -separation;
			++cp;
		}
		else
			m->penetration = 0;

		separation = dot(refFaceNormal, incidentFace[1]) - refC;
		if (separation <= 0.0f)	{
			m->contacts[cp] = incidentFace[1];

			m->penetration += -separation;
			++cp;

			m->penetration /= (float)cp;
		}

		m->contactCount = cp;
	}
}