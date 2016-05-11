#pragma once

#include <cmath>
#include <cassert>
#include <algorithm>


const float PI = 3.141592741f;
const float EPSILON = 0.0001f;

namespace GamaGameEngine {

	struct vec2	{
		union {
			float m[1][1];
			float v[2];

			struct {
				float x;
				float y;
			};
		};

		vec2() {}
		vec2(float a, float b) : x(a) , y(b) {}

		void set(float a, float b) { x = a;	y = b; }

		vec2 operator +(float a) const {
			return vec2(x + a, y + a);
		}

		vec2 operator +(const vec2& rhs) const {
			return vec2(x + rhs.x, y + rhs.y);
		}

		void operator +=(const vec2& rhs) {
			x += rhs.x;
			y += rhs.y;
		}

		vec2 operator -(const vec2& rhs) const {
			return vec2(x - rhs.x, y - rhs.y);
		}

		void operator -=(const vec2& rhs) {
			x -= rhs.x;
			y -= rhs.y;
		}

		vec2 operator -(void) const {
			return vec2(-x, -y);
		}

		vec2 operator *(float a) const {
			return vec2(x * a, y * a);
		}

		void operator *=(float a) {
			x *= a;
			y *= a;
		}

		vec2 operator /(float a) const {
			return vec2(x / a, y / a);
		}

		float length(void) const {
			return std::sqrt(x * x + y * y);
		}

		float lengthSquared(void) const {
			return x * x + y * y;
		}

		void rotate(float radians) {
			float c = std::cos(radians);
			float s = std::sin(radians);

			float xp = x * c - y * s;
			float yp = x * s + y * c;

			x = xp;
			y = yp;
		}

		void normalise(void) {
			float len = length();

			if (len > EPSILON) {
				float invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
			}
		}
	};

	inline vec2 operator*(float s, const vec2& v) {
		return vec2(s * v.x, s * v.y);
	}

	struct mat2	{
		union {
			struct {
				float m00, m01;
				float m10, m11;
			};

			float m[2][2];
			float v[4];
		};

		mat2() {}
		mat2(float radians)	{
			float c = std::cos(radians);
			float s = std::sin(radians);

			m00 = c; 
			m01 = -s;
			m10 = s;
			m11 = c;
		}

		mat2(float a, float b, float c, float d)
			: m00(a), m01(b), m10(c), m11(d) {}

		void set(float radians) {
			float c = std::cos(radians);
			float s = std::sin(radians);

			m00 = c; 
			m01 = -s;
			m10 = s;
			m11 = c;
		}

		mat2 abs(void) const {
			return mat2(std::abs(m00), std::abs(m01), std::abs(m10), std::abs(m11));
		}

		vec2 AxisX(void) const	{
			return vec2(m00, m10);
		}

		vec2 AxisY(void) const	{
			return vec2(m01, m11);
		}

		mat2 transpose(void) const {
			return mat2(m00, m10, m01, m11);
		}

		const vec2 operator *(const vec2& rhs) const {
			return vec2(m00 * rhs.x + m01 * rhs.y, m10 * rhs.x + m11 * rhs.y);
		}

		const mat2 operator*(const mat2& rhs) const {

			return mat2(
				m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
				m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
				m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
				m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
				);
		}
	};

	inline vec2 Min(const vec2& a, const vec2& b) {
		return vec2(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	inline vec2 Max(const vec2& a, const vec2& b) {
		return vec2(std::max(a.x, b.x), std::max(a.y, b.y));
	}

	inline float dot(const vec2& a, const vec2& b) {
		return a.x * b.x + a.y * b.y;
	}

	inline float DistSqr(const vec2& a, const vec2& b) {
		vec2 c = a - b;
		return dot(c, c);
	}

	inline vec2 cross(const vec2& v, float a) {
		return vec2(a * v.y, -a * v.x);
	}

	inline vec2 cross(float a, const vec2& v) {
		return vec2(-a * v.y, a * v.x);
	}

	inline float cross(const vec2& a, const vec2& b) {
		return a.x * b.y - a.y * b.x;
	}

	inline bool equal(float a, float b)	{
		return std::abs(a - b) <= EPSILON;
	}

	inline float Sqr(float a) {
		return a * a;
	}
	
	inline float Clamp(float min, float max, float a) {
		if (a < min) return min;
		if (a > max) return max;
		return a;
	}

	inline int Round(float a) {
		return (int)(a + 0.5f);
	}

	inline float Random(float l, float h) {
		float a = (float)rand();
		a /= RAND_MAX;
		a = (h - l) * a + l;
		return a;
	}

	inline bool BiasGreaterThan(float a, float b) {
		const float k_biasRelative = 0.95f;
		const float k_biasAbsolute = 0.01f;
		return a >= b * k_biasRelative + a * k_biasAbsolute;
	}

	const float gravityScale = 5.0f;
	const vec2 gravity(0, 10.0f * gravityScale);
	const float dt = 1.0f / 60.0f;

}