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

	class Glyph {
	public:

		Glyph() {}
		Glyph(const glm::vec4 & destRect, const glm::vec4 & uvRect, GLuint Texture, float Depth, const ColorRGBA8 & color) :
			texture(Texture),	// Captical Texture and 
			depth(Depth)		// Depth because of naming conflict
		{
			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
		}

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

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);

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

		GlyphSortType m_sortType;

		std::vector<Glyph*> m_glyphPointers;	///< This is for sorting
		std::vector<Glyph> m_glyphs;			///< These are the actual glyphs
		std::vector<RenderBatch> m_renderBatches;
	};
}