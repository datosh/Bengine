#pragma once

#include "stdafx.h"
#include "Agent.h"
#include "Human.h"
#include "Zombie.h"
#include <glm.hpp>
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>

class Human;
class Zombie;

const int BULLET_RADIUS = 5;

class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();

	bool update(const std::vector<std::string>& levelData);

	void draw(Bengine::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent * agent);

	float getDamage() const { return m_damage; }

private:

	bool collideWithWorld(const std::vector<std::string>& levelData);

	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_damage;
	float m_speed;
};

