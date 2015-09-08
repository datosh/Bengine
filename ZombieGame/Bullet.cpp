#include "Bullet.h"



Bullet::Bullet() : Entity()
{
	m_lifeTime = 0;
}

Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color) :
	Entity(pos, dir, speed, size, color)
{
	m_lifeTime = DEFAULT_LIFETIME;
}

Bullet::~Bullet()
{
}

bool Bullet::moveRel(glm::vec2 delta)
{
	m_pos += delta;
	return false;
}

bool Bullet::moveAbs(glm::vec2 pos)
{
	m_pos = pos;
	return false;
}

void Bullet::draw(Bengine::SpriteBatch& spriteBatch)
{
	glm::vec4 posAndSize = glm::vec4(m_pos.x, m_pos.y, m_size, m_size);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture m_texture = Bengine::ResourceManager::getTexture("Textures/circle.png");
	spriteBatch.draw(posAndSize, uv, m_texture.id, 0.0f, m_color);
}

bool Bullet::update()
{
	moveRel(m_dir * m_speed);

	if (!m_lifeTime--)
	{
		m_alive = false;
	}

	return true;
}