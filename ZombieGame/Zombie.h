#pragma once

#include "stdafx.h"
#include "Entity.h"
#include "Player.h"

class Zombie : public Entity
{
public:
	Zombie(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color);
	~Zombie();

	bool moveRel(glm::vec2 delta);
	bool moveAbs(glm::vec2 pos);
	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();

	void follow(Player * entity);
	bool hasTarget();

private:
	bool m_hasTarget;
	Player * m_toFollow;
};

