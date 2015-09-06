#include "Entity.h"



Entity::Entity()
{
	m_pos = { 0.0f, 0.0f };
	m_dir = { 0.0f, 0.0f };
	m_speed = 0.0f;
	m_size = 0.0f;
	m_color = { 0, 0, 0 };
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
