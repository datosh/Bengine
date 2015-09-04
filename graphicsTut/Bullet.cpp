#include "Bullet.h"


Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
{
	m_position = pos;
	m_direction = dir;
	m_speed = speed;
	m_lifeTime = lifeTime;
}

Bullet::~Bullet()
{
}

void Bullet::draw(Bengine::SpriteBatch & spriteBatch)
{
	glm::vec4 posAndSize = glm::vec4(m_position.x, m_position.y, 30, 30);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color color = { 255, 255, 255, 255 };
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/PNG/CharacterRight_Walk2.png");
	spriteBatch.draw(posAndSize, uv, texture.id, 0.0f, color);
	
}

bool Bullet::update()
{
	m_position += m_direction * m_speed;
	if (m_lifeTime-- == 0)
	{
		return true;
	}
	return false;
}
