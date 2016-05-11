#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "DataTypes.h"

namespace GamaGameEngine {

	enum class SpriteSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	class Sprite {

	public:
		Sprite() {};
		Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& colour);
		Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& colour, float angle);

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;

	private:
		glm::vec2 rotateOnPoint(glm::vec2 point, float angle);
	};
}