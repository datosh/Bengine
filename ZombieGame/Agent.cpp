#include "Agent.h"
#include "Level.h"
#include <algorithm>


Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID = Bengine::ResourceManager::getTexture("Textures/circle.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, AGENT_WIDTH, AGENT_WIDTH);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, m_color);
}

bool Agent::applyDamage(float damage)
{
	m_health -= damage;
	return m_health <= 0;
}

bool Agent::collideWithlevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePositions;

	checkTilePosition(levelData, collideTilePositions, 
		m_position.x,
		m_position.y);

	checkTilePosition(levelData, collideTilePositions,
		m_position.x + AGENT_WIDTH,
		m_position.y);

	checkTilePosition(levelData, collideTilePositions,
		m_position.x,
		m_position.y + AGENT_WIDTH);

	checkTilePosition(levelData, collideTilePositions,
		m_position.x + AGENT_WIDTH,
		m_position.y + AGENT_WIDTH);

	if (collideTilePositions.size() == 0)
	{
		return false;
	}

	// TODO: Sort tiles by distance to player to fix sliding bug
	for (auto c : collideTilePositions)
	{
		collideWithTile(c);
	}
	return true;
}

bool Agent::collideWithAgent(Agent * agent)
{
	const float MIN_DISTANCE = AGENT_RADIUS * 2.0f;

	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);

	glm::vec2 distVec = centerPosA - centerPosB;

	float distance = glm::length(distVec);

	float collisionDepth = MIN_DISTANCE - distance;

	if (collisionDepth > 0)
	{

		glm::vec2 collisonsDepthVec = glm::normalize(distVec) * collisionDepth;

		m_position += collisonsDepthVec / 2.0f;
		agent->m_position -= collisonsDepthVec / 2.0f;
		return true;
	}
	return false;
	
	
}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, 
	std::vector<glm::vec2>& collideTilePositions, 
	float x, float y)
{
	// Get the position of this corner in grid-space
	glm::vec2 gridPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH));

	// If we are outside the world, just return
	if (gridPos.x < 0 || gridPos.x >= levelData[0].size() ||
		gridPos.y < 0 || gridPos.y >= levelData.size()) {
		return;
	}

	// If this is not an air tile, we should collide with it
	if (levelData[gridPos.y][gridPos.x] != '.') {
		collideTilePositions.push_back(gridPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}

void Agent::collideWithTile(glm::vec2 tilePos)
{
	
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;

	glm::vec2 centerPlayerPos = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPlayerPos - tilePos; 
	
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 && yDepth > 0)
	{
		if ((std::max)(xDepth, 0.0f) < (std::max)(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				m_position.x -= xDepth;
			}
			else
			{
				m_position.x += xDepth;
			}
		}
		else
		{
			if (distVec.y < 0)
			{
				m_position.y -= yDepth;
			}
			else
			{
				m_position.y += yDepth;
			}
		}
	}
}
