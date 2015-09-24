#include "Player.h"

#include "Gun.h"

Player::Player() :
	m_currentGunIndex(-1)
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, 
	Bengine::InputManager* inputManager, 
	Bengine::Camera2D* camera,
	std::vector<Bullet>* bullets)
{
	m_speed = speed;
	m_position = position;
	m_inputManager = inputManager;
	m_camera = camera;
	m_bullets = bullets;

	m_color = Bengine::ColorRGBA8(0, 0, 127, 255);

	m_health = 150;
}

void Player::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	if (m_inputManager->isKeyDown(SDLK_w))
	{
		m_position.y += m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_s))
	{
		m_position.y -= m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_a))
	{
		m_position.x -= m_speed * deltaTime;
	}
	else if (m_inputManager->isKeyDown(SDLK_d))
	{
		m_position.x += m_speed * deltaTime;
	}

	if (m_inputManager->isKeyDown(SDLK_1) && m_guns.size() >= 0)
	{
		m_currentGunIndex = 0;
	}
	else if (m_inputManager->isKeyDown(SDLK_2) && m_guns.size() >= 1)
	{
		m_currentGunIndex = 1;
	}
	else if (m_inputManager->isKeyDown(SDLK_3) && m_guns.size() >= 2)
	{
		m_currentGunIndex = 2;
	}

	if (m_currentGunIndex != -1)
	{
		glm::vec2 mouseCoords = m_inputManager->getMouseCoords();
		mouseCoords = m_camera->convertScreenToWorld(mouseCoords);

		glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);

		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);

		m_guns[m_currentGunIndex]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT),
			centerPosition,
			direction,
			*m_bullets,
			deltaTime);
	}

	collideWithlevel(levelData);
}

void Player::addGun(Gun * gun)
{
	// Add gun to inventory
	m_guns.push_back(gun);

	// if no gun equipped, equip gun
	if (m_currentGunIndex == -1)
	{
		m_currentGunIndex = 0;
	}
}
