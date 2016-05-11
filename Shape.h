#pragma once

#include "RigidBody.h"
#include "GamaMath.h"
#include <GL/glut.h>

#define MaxPolyVertexCount 64

namespace GamaGameEngine {

	class RigidBody;
	class Shape {

	public:
		enum Type { CIRCLE, POLYGON, COUNT };

		Shape() {}
		virtual Shape* clone(void) const = 0;
		virtual void init(void) = 0;
		virtual void computeMass(float density) = 0;
		virtual void setOrient(float radians) = 0;
		virtual void draw(void) const = 0;
		virtual Type getType(void) const = 0;
	
		RigidBody* m_body;

		// For circle 
		float m_radius;

		// For Polygon 
		mat2 m_orientation;
	};

	class Circle : public Shape {

	public:
		Circle(float r) { m_radius = r;	}

		Shape* clone(void) const { return new Circle(m_radius);	}

		void init(void) { computeMass(1.0f); }

		void computeMass(float density); 

		void setOrient(float radians) {
			//Empty
		}

		void draw(void) const;

		Type getType(void) const { return CIRCLE; }
	};

	class Polygon : public Shape {
	public:

		void init(void) { computeMass(1.0f); }

		Shape* clone(void) const;

		void computeMass(float density);

		void setOrient(float radians) {	m_orientation.set(radians); }

		void draw(void) const;

		Type getType(void) const { return POLYGON; }

		// Half width and half height
		void setBox(float hw, float hh);

		void set(vec2* vertices, int count);

		vec2 getSupport(const vec2& dir);

		int m_vertexCount;
		vec2 m_vertices[MaxPolyVertexCount];
		vec2 m_normals[MaxPolyVertexCount];
	};
}