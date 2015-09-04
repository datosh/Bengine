#pragma once

#include "stdafx.h"

#include <glm.hpp>
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>

class Bullet
{
public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Bullet();

	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();

private:
	int m_lifeTime;
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
};

