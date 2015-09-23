#include "Bullet.h"

#include "Level.h"

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) :
	m_position(position),
	m_direction(direction),
	m_damage(damage),
	m_speed(speed)
{
}

Bullet::~Bullet()
{
}

bool Bullet::update(const std::vector<std::string>& levelData)
{
	m_position += m_direction * m_speed;

	return collideWithWorld(levelData);
}

void Bullet::draw(Bengine::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect(m_position.x + BULLET_RADIUS, 
		m_position.y + BULLET_RADIUS, 
		BULLET_RADIUS * 2, 
		BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	Bengine::Color color;
	color.r = 50;
	color.g = 50;
	color.b = 50;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect,
		Bengine::ResourceManager::getTexture("Textures/circle.png").id,
		0.0f,
		color);
}

bool Bullet::collideWithAgent(Agent * agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;

	glm::vec2 centerPosA = m_position;
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{
		return true;
	}
	return false;
}

bool Bullet::collideWithWorld(const std::vector<std::string>& levelData)
{
	glm::ivec2 gridPostion;

	gridPostion.x = static_cast<int>(floor(m_position.x / TILE_WIDTH));
	gridPostion.y = static_cast<int>(floor(m_position.y / TILE_WIDTH));

	if (gridPostion.x < 0 || gridPostion.x >= levelData[0].length() ||
		gridPostion.y < 0 || gridPostion.y >= levelData.size())
	{
		return true;
	}

	return levelData[gridPostion.y][gridPostion.x] != '.';
}
