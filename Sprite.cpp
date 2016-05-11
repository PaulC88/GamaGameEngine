#include "Sprite.h"

#include <algorithm>

namespace GamaGameEngine {

	//Constructor for permanent sprites 
	Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& colour) :
		texture(Texture), depth(Depth) {

		topLeft.colour = colour;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.colour = colour;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.colour = colour;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.colour = colour;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	//Constructor for rotating sprites 
	Sprite::Sprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Colour& colour, float angle) :
		texture(Texture), depth(Depth) {

		//z is width - w is height
		glm::vec2 halfDimensions(destRect.z / 2.0f, destRect.w / 2.0f);

		//Get points centered at origin
		//4 vertices representing our sprite centered on the origin for rotation
		glm::vec2 topL(-halfDimensions.x, halfDimensions.y);
		glm::vec2 bottomL(-halfDimensions.x, -halfDimensions.y);
		glm::vec2 bottomR(halfDimensions.x, -halfDimensions.y);
		glm::vec2 topR(halfDimensions.x, halfDimensions.y);

		//Rotate the points
		topL = rotateOnPoint(topL, angle) + halfDimensions;
		bottomL = rotateOnPoint(bottomL, angle) + halfDimensions;
		bottomR = rotateOnPoint(bottomR, angle) + halfDimensions;
		topR = rotateOnPoint(topR, angle) + halfDimensions;

		topLeft.colour = colour;
		topLeft.setPosition(destRect.x + topL.x, destRect.y + topL.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.colour = colour;
		bottomLeft.setPosition(destRect.x + bottomL.x, destRect.y + bottomL.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.colour = colour;
		bottomRight.setPosition(destRect.x + bottomR.x, destRect.y + bottomR.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.colour = colour;
		topRight.setPosition(destRect.x + topR.x, destRect.y + topR.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	//Rotate sprite on a point
	glm::vec2 Sprite::rotateOnPoint(glm::vec2 point, float angle) {
		glm::vec2 newVec;
		newVec.x = point.x * cos(angle) - point.y * sin(angle);
		newVec.y = point.x * sin(angle) + point.y * cos(angle);
		return newVec;
	}
}