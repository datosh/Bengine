#include "entity.h"

Entity::Entity()
{
	// Empty
}

Entity::~Entity()
{
	// Empty
}

bool Entity::collideWithLevel(const Level & level)
{
	return false;
}

bool Entity::collideWithEntity(const std::vector<Entity>& entitys)
{
	return false;
}

/// Do a basic AABB collision check. If the enity needs more sophisticated
/// collision detection it can override this method
bool Entity::collideWithEntity(const Entity & entity)
{
	float ax = m_position.x;
	float ay = m_position.y;
	float aw = m_width;
	float ah = m_height;
	float bx = entity.m_position.x;
	float by = entity.m_position.y;
	float bw = entity.m_width;
	float bh = entity.m_height;

	if (ax < bx + bw &&
		ax + aw > bx &&
		ay < by + bh &&
		ah + ay > by) return true;

	return false;
}
