#include "sprite.h"

namespace Bengine {
	Sprite::Sprite()
	{
		m_vboID = 0;
	}


	Sprite::~Sprite()
	{
		if (m_vboID != 0)
		{
			glDeleteBuffers(1, &m_vboID);
		}
	}

	void Sprite::init(float x, float y, float width, float height, std::string texturePath)
	{
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;

		m_texture = ResourceManager::getTexture(texturePath);

		if (m_vboID == 0)
		{
			glGenBuffers(1, &m_vboID);
		}

		Vertex vertexData[] = {
			{ { x + width, y + height}, {3, 0, 255, 255}, {1.0f, 1.0f} },
			{ { x, y + height}, { 255, 55, 255, 255 }, { 0.0f, 1.0f } },
			{ { x, y}, { 255, 0, 33, 255 }, { 0.0f, 0.0f } },
			{ { x, y}, { 66, 255, 255, 255 }, { 0.0f, 0.0f } },
			{ { x + width, y}, { 255, 44, 255, 255 }, { 1.0f, 0.0f } },
			{ { x + width, y + height}, { 135, 0, 255, 255 }, { 1.0f, 1.0f } },
		};

		// Bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		// Upload the buffer to GPU
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
		// Unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Sprite::draw()
	{
		glBindTexture(GL_TEXTURE_2D, m_texture.id);

		glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
		glEnableVertexAttribArray(0);

		// This is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		// This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		// This is the uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}