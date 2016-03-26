#include "ball.h"

#include "mainGame.h"
#include "utility.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::init(MainGame * mainGame)
{
	m_speed = 6.5f;
	m_width = 20;
	m_height = 20;

	m_alive = true;

	m_position.x = -m_width / 2 + mainGame->get_width() / 2;
	m_position.y = 50;

	m_velocity.x = .2f;
	m_velocity.y = .5f;
	m_velocity = glm::normalize(m_velocity);
}

void Ball::update(MainGame * mainGame, float deltaTime)
{
	// Move forward
	m_position += (m_speed * m_velocity) * deltaTime;

	// Get screen size for convenience
	auto screen_width = mainGame->get_width();
	auto screen_height = mainGame->get_height();

	// Bounce of the left wall
	if (m_position.x < m_width / 4 && m_velocity.x < 0) {
		m_velocity.x *= -1;
	}
	// Bounce of the right wall
	if (m_position.x > screen_width - m_width && m_velocity.x > 0) {
		m_velocity.x *= -1;
	}
	// Bounce of the top wall
	if (m_position.y > screen_height - m_height && m_velocity.y > 0) {
		m_velocity.y *= -1;
	}
	// Bounce of the paddle
	Paddle* paddle = mainGame->get_paddle();
	if (collideWithEntity(paddle) && m_velocity.y < 0) {
		
		m_velocity.y *= -1;

		auto paddle_pos = paddle->get_position();
		// get distance of the center of the paddle and ball
		auto delta_x = (m_position.x + m_width / 2) - (paddle_pos.x + paddle->get_width() / 2);
		auto velo_change = delta_x * delta_x * delta_x * 0.000002f;

		// Change the velocity depending on hitting spot
		m_velocity.x += velo_change;

		// Only keep direction, not speed
		m_velocity = glm::normalize(m_velocity);
	}

	// Bounce of the any bricks and destroy them
	if (Entity* collidedEntity = collideWithEntities(mainGame->get_bricks())) {
		Brick* collidedBrick = dynamic_cast<Brick*>(collidedEntity);
		
		// Check where the destroyed brick is relative to the ball
		auto brick_pos = collidedBrick->get_position();
		auto brick_size = collidedBrick->get_size();

		// Ball below brick
		if (below(collidedEntity)) {
			std::cout << "below" << std::endl;

			// Move down if not already -> multi kill possible
			if(m_velocity.y > 0) m_velocity.y *= -1;
		}
		// Ball above brick
		else if (above(collidedEntity)) {
			std::cout << "above" << std::endl;

			// Move up if not already -> multi kill possible
			if(m_velocity.y < 0) m_velocity.y *= -1;
		}
		// Ball left of brick
		else if (left_of(collidedEntity)) {
			std::cout << "left" << std::endl;

			// Move left if not already -> multi kill possible
			if(m_velocity.x < 0) m_velocity.x *= -1;
		}
		// Ball right of brick
		else if (right_of(collidedEntity)) {
			std::cout << "right" << std::endl;

			// Move right if not already -> multi kill possible
			if(m_velocity.x > 0) m_velocity.x *= -1;
		}
		else {
			std::cerr << "UTIL FUZZY COMPARE BUGGY. THROWN IN BALL.CPP BRICK COLLISION" << std::endl;
		}

		// Add the points for the destroyed brick
		mainGame->add_points(collidedBrick->get_points());

		// Maybe spawn a power_up
		float chance = mainGame->get_powerUpChance();
		float randfloat = util::randFloat(0.0f, 1.0f);
		if (chance > randfloat) { // Yes we cant a powerup spawn
			mainGame->add_random_power_up(collidedBrick->get_position());
		}

		// Spawn the explosion particles
		mainGame->spawn_brick_paricles(collidedBrick->get_position());

		// Mark brick for remove
		collidedBrick->kill();
	}

	// If we leave the bottom screen -> game over
	if (m_position.y < 0) {
		m_alive = false;
	}
}

void Ball::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID = Bengine::ResourceManager::getTexture("Textures/circle.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, m_width, m_height);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, Bengine::ColorRGBA8(235, 123, 2, 255));
}