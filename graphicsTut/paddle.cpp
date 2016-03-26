#include "paddle.h"
#include "mainGame.h"


Paddle::Paddle()
{
}

Paddle::~Paddle()
{
}

void Paddle::init(MainGame* mainGame)
{
	m_speed = 6.5f;
	m_width = 150.0f;
	m_height = 25.0f;

	m_to_grow = 0.0f;
	m_max_width = 350.0f;

	m_position.x = -m_width / 2 + mainGame->get_width() / 2;
	m_position.y = 0.0f;
}

void Paddle::update(MainGame* mainGame, float deltaTime)
{
	auto coords = m_inputManager->getMouseCoords();
	auto transcoords = mainGame->get_mainCamera().convertScreenToWorld(coords);
	
	// Handle movement
	if (m_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed * deltaTime;
	}
	// Keep paddle inside the screen
	// Screen is in range [-screenwidth;screenwidth]
	if (m_position.x < 0)
		m_position.x = 0;
	if (m_position.x > mainGame->get_width() - m_width) 
		m_position.x = mainGame->get_width() - m_width;

	// Check for collision with powerup
	if (Entity* collided_entity = collideWithEntities(mainGame->get_powerUps())) {
		PowerUp* collided_power_up = dynamic_cast<PowerUp*>(collided_entity);
		auto pu_name = collided_power_up->get_name();

		if (pu_name == "paddle_size") {
			m_to_grow = 30.0f;
		}
		else if (pu_name == "times_two") {
			mainGame->double_balls();
		}
		else if (pu_name == "speed_up") {
			m_speed += 2.0f;
		}

		collided_power_up->kill();
	}


	// Grow if needed => size power up active
	if (m_to_grow > 0) {
		static float grow_delta = 0.8f;

		if (m_to_grow > grow_delta) {
			m_width += grow_delta;
			m_to_grow -= grow_delta;
			m_position.x -= grow_delta / 2;
		}
		else {
			float remaining_grow = m_to_grow;
			m_to_grow = 0;
			m_width += remaining_grow;
			m_position.x -= remaining_grow / 2;
		}
	}
}

void Paddle::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID = Bengine::ResourceManager::getTexture("Textures/brick.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, m_width, m_height);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, Bengine::ColorRGBA8(2, 29, 39, 255));
}
