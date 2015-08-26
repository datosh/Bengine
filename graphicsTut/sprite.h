#pragma once

#include "stdafx.h"

#include "Vertex.h"

#include <glew.h>

class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(float x, float y, float width, float height);
	void draw();

private:
	float m_x, m_y, m_width, m_height;
	GLuint m_vboID;
};

