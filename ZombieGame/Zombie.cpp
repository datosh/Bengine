#include "Zombie.h"


Zombie::Zombie() : Entity()
{
}

Zombie::Zombie(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color) :
	Entity(pos, dir, speed, size, color)
{
}


Zombie::~Zombie()
{
}

bool Zombie::moveRel(glm::vec2 delta)
{
	m_pos += delta;

	return false;
}

bool Zombie::moveAbs(glm::vec2 pos)
{
	return false;
}

void Zombie::draw(Bengine::SpriteBatch & spriteBatch)
{
	glm::vec4 posAndSize = glm::vec4(m_pos.x, m_pos.y, m_size, m_size);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture m_texture = Bengine::ResourceManager::getTexture("Textures/circle.png");
	spriteBatch.draw(posAndSize, uv, m_texture.id, 0.0f, m_color);
}

bool Zombie::update()
{
	static const float DIST_THRESH = 150;

	glm::vec2 dist_vec = m_toFollow->getPos() - m_pos;
	float distance = glm::length(dist_vec);

	if (distance > DIST_THRESH)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dirDis(-1, 1);
		m_dir = { dirDis(gen), dirDis(gen) };
	}
	else
	{
		m_dir = glm::normalize(m_toFollow->getPos() - m_pos);
	}

	moveRel(m_dir * m_speed);

	return false;
}

void Zombie::follow(Player * entity)
{
	m_toFollow = entity;
	m_hasTarget = true;
}

bool Zombie::hasTarget()
{
	return m_hasTarget;
}