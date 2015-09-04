#include "Player.h"

Player::Player(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color) :
	Entity(pos, dir, speed, size, color)
{
	
}

Player::~Player()
{
}

void Player::draw(Bengine::SpriteBatch & spriteBatch)
{
	glm::vec4 posAndSize = glm::vec4(m_pos.x, m_pos.y, m_size, m_size);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture m_texture = Bengine::ResourceManager::getTexture("Textures/circle.png");
	spriteBatch.draw(posAndSize, uv, m_texture.id, 0.0f, m_color);
}

bool Player::moveRel(glm::vec2 delta)
{
	m_pos += delta;
	return true;
}

bool Player::moveAbs(glm::vec2 pos)
{
	m_pos = pos;
	return true;
}

bool Player::update()
{
	return true;
}
