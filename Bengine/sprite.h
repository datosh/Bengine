#pragma once

#include "stdafx.h"
#include "Vertex.h"
#include "GLTexture.h"
#include "ResourceManager.h"

#include <glew.h>

namespace Bengine {
	class Sprite
	{
	public:
		Sprite();
		~Sprite();

		void init(float x, float y, float width, float height, std::string texturePath);
		void draw();

	private:
		float m_x, m_y;
		float m_width, m_height;
		GLuint m_vboID;
		GLTexture m_texture;
	};
}
