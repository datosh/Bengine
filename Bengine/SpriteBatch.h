#pragma once

#include "stdafx.h"
#include "Vertex.h"

#include <glew.h>
#include <glm.hpp>

namespace Bengine {

	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	struct Glyph {
		GLuint texture;
		float depth;
		
		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) :
			m_offset(offset), m_numVertices(numVertices), m_texture(texture) {}

		GLuint m_offset;
		GLuint m_numVertices;
		GLuint m_texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		
		void init();

		void begin(GlyphSortType sortType=GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);

		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlpyhs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint m_vbo;
		GLuint m_vao;
		std::vector<Glyph*> m_glyphs;
		std::vector<RenderBatch> m_renderBatches;
		GlyphSortType m_sortType;

	};
}