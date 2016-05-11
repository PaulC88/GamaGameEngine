/*The regular way to draw things onto the screen (using love.graphics.draw) is to send the image and 
position to the GPU every time you draw anything, even if you're using the same image multiple times in a row. 
Sending information to the GPU can become a huge bottleneck, especially if that information doesn't actually need to be sent. 
Spritebatches help with this problem by specifying an image to draw and then saving all the locations you want to draw it
at to the GPU's VRAM for use at a later time. Then it only has to send the image once and tell the GPU to use the locations 
it saved. This can have huge performance benefits if, for example, you have an image of background tiles,
because then you can just make a spritebatch with the image, add the locations of the tile quads, and then 
draw the entire thing in one go at a later time, using the saved position data, instead of re-sending the image and
related data over and over and over.*/


#include "SpriteBatch.h"

#include <algorithm>

namespace GamaGameEngine {

	SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0)	{
		//Empty
	}


	SpriteBatch::~SpriteBatch()	{
		//Empty
	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	//Start sprite batch
	void SpriteBatch::begin(SpriteSortType sortType) {
		m_sortType = sortType;
		m_renderBatches.clear();
		
		m_sprites.clear();
	}

	//Add sprite to batch
	void SpriteBatch::addSprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour& colour) {

		m_sprites.emplace_back(destRect, uvRect, texture, depth, colour);
	}

	//Add sprite to batch with angle
	void SpriteBatch::addSprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour& colour, float angle) {
		m_sprites.emplace_back(destRect, uvRect, texture, depth, colour, angle);
	}

	//Add sprite to batch with direction
	void SpriteBatch::addSprite(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Colour& colour, const glm::vec2& direction) {
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, direction));

		if (direction.y < 0.0f) 
			angle = -angle;

		m_sprites.emplace_back(destRect, uvRect, texture, depth, colour, angle);
	}

	//Finish batch creation
	void SpriteBatch::end() {
		m_spritePointers.resize(m_sprites.size());
		for (unsigned int i = 0; i < m_sprites.size(); i++) {
			m_spritePointers[i] = &m_sprites[i];
		}
		sortSprites();
		createRenderBatches();
	}

	//Render contents of batch
	void SpriteBatch::renderBatch() {

		glBindVertexArray(m_vao);
		for (unsigned int i = 0; i < m_renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;

		vertices.resize(m_spritePointers.size() * 6);

		if (m_spritePointers.empty()) {
			return;
		}
		int offset = 0;
		int cv = 0;

		m_renderBatches.emplace_back(offset, 6, m_spritePointers[0]->texture);
		vertices[cv++] = m_spritePointers[0]->topLeft;
		vertices[cv++] = m_spritePointers[0]->bottomLeft;
		vertices[cv++] = m_spritePointers[0]->bottomRight;
		vertices[cv++] = m_spritePointers[0]->bottomRight;
		vertices[cv++] = m_spritePointers[0]->topRight;
		vertices[cv++] = m_spritePointers[0]->topLeft;
		offset += 6;

		for (unsigned int cg = 1; cg < m_spritePointers.size(); cg++) {
			if (m_spritePointers[cg]->texture != m_spritePointers[cg - 1]->texture) {

				m_renderBatches.emplace_back(offset, 6, m_spritePointers[cg]->texture);
			}
			else {
				m_renderBatches.back().numVertices += 6;
			}
			vertices[cv++] = m_spritePointers[cg]->topLeft;
			vertices[cv++] = m_spritePointers[cg]->bottomLeft;
			vertices[cv++] = m_spritePointers[cg]->bottomRight;
			vertices[cv++] = m_spritePointers[cg]->bottomRight;
			vertices[cv++] = m_spritePointers[cg]->topRight;
			vertices[cv++] = m_spritePointers[cg]->topLeft;
			offset += 6;
		}


		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);	//vertices.data = same as saying the address of the first element
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	
	void SpriteBatch::createVertexArray() {

		if (m_vao == 0) {
			glGenVertexArrays(1, &m_vao);
		}

		glBindVertexArray(m_vao);

		if (m_vbo == 0) {
			glGenBuffers(1, &m_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);


		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	//Sort sprites by type
	void SpriteBatch::sortSprites() {
		switch (m_sortType) {
		case SpriteSortType::BACK_TO_FRONT:
			std::stable_sort(m_spritePointers.begin(), m_spritePointers.end(), compareBackToFront);
			break;
		case SpriteSortType::FRONT_TO_BACK:
			std::stable_sort(m_spritePointers.begin(), m_spritePointers.end(), compareFrontToBack);
			break;
		case SpriteSortType::TEXTURE:
			std::stable_sort(m_spritePointers.begin(), m_spritePointers.end(), compareTexture);
			break;
		}

	}

	bool SpriteBatch::compareFrontToBack(Sprite* a, Sprite* b) {
		return (a->depth < b->depth);
	}
	
	bool SpriteBatch::compareBackToFront(Sprite* a, Sprite* b) {
		return (a->depth > b->depth);
	}
	
	bool SpriteBatch::compareTexture(Sprite* a, Sprite* b) {
		return (a->texture < b->texture);
	}
}