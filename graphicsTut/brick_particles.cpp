#include "brick_particles.h"

#include "utility.h"

BrickParticle::BrickParticle()
{
}

BrickParticle::~BrickParticle()
{
}

void BrickParticle::init(MainGame * mainGame)
{
	// A brick is 100, 50 => we make 8 brick paricles each 25x25
	m_width = 25.0f;
	m_height = 25.0f;
	m_velocity = glm::vec2(util::randFloat(-1.0f, 1.0f), util::randFloat(-1.0f, 1.0f));
	m_velocity = glm::normalize(m_velocity);
	m_shrink_factor = 1.5f;
	m_speed = 3.0f;
}

void BrickParticle::update(MainGame * mainGame, float deltaTime)
{
	m_position += m_speed * m_velocity * deltaTime;
	m_width -= m_shrink_factor * deltaTime;
	m_height -= m_shrink_factor * deltaTime;

	// Mark for deletion if not longer visible
	if (m_height < 1 || m_width < 1) {
		kill();
	}
}

void BrickParticle::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID = Bengine::ResourceManager::getTexture("Textures/brick.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, m_width, m_height);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, Bengine::ColorRGBA8(2, 29, 39, 255));
}
