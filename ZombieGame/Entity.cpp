#include "Entity.h"



Entity::Entity()
{
}

Entity::Entity(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color) :
	m_pos(pos),
	m_dir(dir),
	m_speed(speed),
	m_size(size),
	m_color(color)
{

}

Entity::~Entity()
{
}

bool Entity::overlaps(Entity & entity)
{
	float Ax = m_pos.x;
	float Ay = m_pos.y;
	float AX = m_pos.x + m_size;
	float AY = m_pos.y + m_size;

	glm::vec2 otherPos = entity.getPos();
	float Bx = otherPos.x;
	float By = otherPos.y;
	float BX = otherPos.x + entity.getSize();
	float BY = otherPos.y + entity.getSize();

	return !(AX < Bx || BX < Ax || AY < By || BY < Ay);
}
