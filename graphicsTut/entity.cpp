#include "entity.h"

#include "utility.h"

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

Entity* Entity::collideWithEntities(const std::vector<Entity*>& entitys)
{
	for (Entity* entity : entitys) {
		if (collideWithEntity(entity)) {
			return entity;
		}
	}
	return nullptr;
}

/// Do a basic AABB collision check. If the enity needs more sophisticated
/// collision detection it can override this method
bool Entity::collideWithEntity(const Entity* entity)
{
	float ax = m_position.x;
	float ay = m_position.y;
	float aw = m_width;
	float ah = m_height;
	float bx = entity->m_position.x;
	float by = entity->m_position.y;
	float bw = entity->m_width;
	float bh = entity->m_height;

	if (ax < bx + bw &&
		ax + aw > bx &&
		ay < by + bh &&
		ah + ay > by) return true;

	return false;
}

bool Entity::below(const Entity * entity, float delta)
{
	return util::ffeq(top(), entity->bottom(), delta) &&	// top needs to be equal to bottom
		util::ffgt(right(), entity->left(), delta) &&		// right needs to be right of left
		util::fflt(left(), entity->right(), delta);			// left needs to be left of right
}

bool Entity::above(const Entity * entity, float delta)
{
	return util::ffeq(bottom(), entity->top(), delta) &&	// bottom needs to be equal to top
		util::ffgt(right(), entity->left(), delta) &&		// right needs to be right of left
		util::fflt(left(), entity->right(), delta);			// left needs to be left of right
}

bool Entity::left_of(const Entity * entity, float delta)
{
	return util::ffeq(right(), entity->left(), delta) &&	// right needs to be equal to left
		util::ffgt(top(), entity->bottom(), delta) &&		// right needs to be right of left
		util::fflt(bottom(), entity->top(), delta);			// left needs to be left of right
}

bool Entity::right_of(const Entity * entity, float delta)
{
	return util::ffeq(left(), entity->right(), delta) &&	// bottom needs to be above top
		util::ffgt(top(), entity->bottom(), delta) &&		// right needs to be right of left
		util::fflt(bottom(), entity->top(), delta);			// left needs to be left of right
}
