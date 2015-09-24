#pragma once

#include <glm.hpp>
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>

const float AGENT_WIDTH = 55.0f;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;

class Zombie;
class Human;

class Agent
{
public:
	Agent();
	virtual ~Agent();

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) = 0;

	void draw(Bengine::SpriteBatch& spriteBatch);

	// Return true if dead
	bool applyDamage(float damage);

	bool collideWithlevel(const std::vector<std::string>& levelData);
	bool collideWithAgent(Agent* agent);

	glm::vec2 getPosition() const { return m_position; }

protected:
	void checkTilePosition(const std::vector<std::string>& levelData,
		std::vector<glm::vec2>& collideTilePositions,
		float x, float y);
	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 m_position;
	Bengine::ColorRGBA8 m_color;
	float m_speed;
	float m_health;
};

