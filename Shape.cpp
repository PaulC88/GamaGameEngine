#include "Shape.h"

namespace GamaGameEngine {

	void Circle::computeMass(float density) {
		m_body->setMass(PI * m_radius * m_radius * density);
		m_body->setInvMass((m_body->getMass()) ? 1.0f / m_body->getMass() : 0.0f);
		m_body->setInertia(m_body->getMass() * m_radius * m_radius);
		m_body->setInvInertia((m_body->getInertia()) ? 1.0f / m_body->getInertia() : 0.0f);
	}

	void Circle::draw(void) const {
		const int circumference = 20;

		glColor3f(m_body->r, m_body->g, m_body->b);
		glBegin(GL_LINE_LOOP);
		float theta = m_body->getOrient();
		float inc = PI * 2.0f / (float)circumference;
		for (int i = 0; i < circumference; ++i) {
			theta += inc;
			vec2 p(std::cos(theta), std::sin(theta));
			p *= m_radius;
			p += m_body->getPosition();
			glVertex2f(p.x, p.y);
		}
		glEnd();
	}

	Shape* Polygon::clone(void) const {
		Polygon* poly = new Polygon();
		poly->m_orientation = m_orientation;
		for (int i = 0; i < m_vertexCount; ++i) {
			poly->m_vertices[i] = m_vertices[i];
			poly->m_normals[i] = m_normals[i];
		}
		poly->m_vertexCount = m_vertexCount;
		return poly;
	}

	void Polygon::computeMass(float density) {
		vec2 centroid(0.0f, 0.0f);
		float area = 0.0f;
		float inertia = 0.0f;
		const float k_inv3 = 1.0f / 3.0f;

		for (int i1 = 0; i1 < m_vertexCount; ++i1) {
			vec2 v1(m_vertices[i1]);
			int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
			vec2 v2(m_vertices[i2]);

			float D = cross(v1, v2);
			float triangleArea = 0.5f * D;

			area += triangleArea;

			centroid += triangleArea * k_inv3 * (v1 + v2);

			float intx2 = v1.x * v1.x + v2.x * v1.x + v2.x * v2.x;
			float inty2 = v1.y * v1.y + v2.y * v1.y + v2.y * v2.y;
			inertia += (0.25f * k_inv3 * D) * (intx2 + inty2);
		}

		centroid *= 1.0f / area;

		for (int i = 0; i < m_vertexCount; ++i)
			m_vertices[i] -= centroid;

		m_body->setMass(density * area);
		m_body->setInvMass((m_body->getMass()) ? 1.0f / m_body->getMass() : 0.0f);
		m_body->setInertia(inertia * density);
		m_body->setInvInertia(m_body->getInertia() ? 1.0f / m_body->getInertia() : 0.0f);
	}


	void Polygon::draw(void) const {
		glColor3f(m_body->r, m_body->g, m_body->b);
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < m_vertexCount; ++i) {
			vec2 v = m_body->getPosition() + m_orientation * m_vertices[i];
			glVertex2f(v.x, v.y);
		}
		glEnd();
	}

	void Polygon::setBox(float hw, float hh) {
		m_vertexCount = 4;
		m_vertices[0].set(-hw, -hh);
		m_vertices[1].set(hw, -hh);
		m_vertices[2].set(hw, hh);
		m_vertices[3].set(-hw, hh);
		m_normals[0].set(0.0f, -1.0f);
		m_normals[1].set(1.0f, 0.0f);
		m_normals[2].set(0.0f, 1.0f);
		m_normals[3].set(-1.0f, 0.0f);
	}

	void Polygon::set(vec2* vertices, int count) {
		assert(count > 2 && count <= MaxPolyVertexCount);
		count = std::min((int)count, MaxPolyVertexCount);

		int rightMost = 0;
		float highestXCoord = vertices[0].x;
		for (int i = 1; i < count; ++i) {
			float x = vertices[i].x;

			if (x > highestXCoord) {
				highestXCoord = x;
				rightMost = i;
			}
			else if (x == highestXCoord)
				if (vertices[i].y < vertices[rightMost].y)
					rightMost = i;
		}

		int hull[MaxPolyVertexCount];
		int outCount = 0;
		int indexHull = rightMost;

		for (;;) {
			hull[outCount] = indexHull;

			int nextHullIndex = 0;
			for (int i = 1; i < (int)count; ++i) {
				if (nextHullIndex == indexHull) {
					nextHullIndex = i;
					continue;
				}

				vec2 e1 = vertices[nextHullIndex] - vertices[hull[outCount]];
				vec2 e2 = vertices[i] - vertices[hull[outCount]];
				float c = cross(e1, e2);
				if (c < 0.0f)
					nextHullIndex = i;

				if (c == 0.0f && e2.lengthSquared() > e1.lengthSquared())
					nextHullIndex = i;
			}

			++outCount;
			indexHull = nextHullIndex;

			if (nextHullIndex == rightMost) {
				m_vertexCount = outCount;
				break;
			}
		}

		for (int i = 0; i < m_vertexCount; ++i)
			m_vertices[i] = vertices[hull[i]];

		for (int i1 = 0; i1 < m_vertexCount; ++i1) {
			int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
			vec2 face = m_vertices[i2] - m_vertices[i1];

			assert(face.lengthSquared() > EPSILON * EPSILON);

			m_normals[i1] = vec2(face.y, -face.x);
			m_normals[i1].normalise();
		}
	}

	vec2 Polygon::getSupport(const vec2& dir) {
		float bestProjection = -FLT_MAX;
		vec2 bestVertex;

		for (int i = 0; i < m_vertexCount; ++i) {
			vec2 v = m_vertices[i];
			float projection = dot(v, dir);

			if (projection > bestProjection) {
				bestVertex = v;
				bestProjection = projection;
			}
		}

		return bestVertex;
	}
}