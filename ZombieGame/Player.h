#pragma once

#include "stdafx.h"
#include "Entity.h"

#include <glm.hpp>
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>
#include <Bengine\Vertex.h>

class Player : public Entity
{
public:
	Player(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color);
	~Player();

	bool moveRel(glm::vec2 delta);
	bool moveAbs(glm::vec2 pos);
	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();

private:
	float m_life;
};

