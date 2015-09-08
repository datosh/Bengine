#pragma once

#include "stdafx.h"

#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet();
	Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color);
	~Bullet();

	bool moveRel(glm::vec2 delta);
	bool moveAbs(glm::vec2 pos);
	void draw(Bengine::SpriteBatch& spriteBatch);
	bool update();

private:
	// TODO: Remove static - why lambda error?????????
	static const int DEFAULT_LIFETIME = 50;
	int m_lifeTime;
};

