#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "DataTypes.h"
#include "Sprite.h"

namespace GamaGameEngine {

	class RenderBatch {
	public:  
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset),
			numVertices(NumVertices), texture(Texture) {

		}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch {

	public:
		SpriteBatch();
		~SpriteBatch();

		void init();

		void begin(SpriteSortType sortType = SpriteSortType::TEXTURE);

		void addSprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour& colour);

		void addSprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour& colour, float angle);

		void addSprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour& colour, const glm::vec2& direction);

		void end();

		void renderBatch();

	private:

		void createRenderBatches();

		void createVertexArray();

		void sortSprites();

		static bool compareFrontToBack(Sprite* a, Sprite* b);
		static bool compareBackToFront(Sprite* a, Sprite* b);
		static bool compareTexture(Sprite* a, Sprite* b);

		GLuint m_vbo;
		GLuint m_vao;

		SpriteSortType m_sortType;
		
		std::vector<Sprite*> m_spritePointers;
		std::vector<Sprite> m_sprites;
		std::vector<RenderBatch> m_renderBatches;
	};


}
