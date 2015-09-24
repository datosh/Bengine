#include "Human.h"

Human::Human() :
	m_frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos)
{
	// Init PRNG
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

	m_speed = speed;
	m_position = pos;
	m_color = Bengine::ColorRGBA8(200, 0, 200, 255);

	m_health = 20;

	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	// If both random numbers are 0 - set to a chosen direction
	if (m_direction.length() == 0)
	{
		m_direction = glm::vec2(1.0f, 0.0f);
	}
	m_direction = glm::normalize(m_direction);
}

void Human::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	m_position += m_direction * m_speed * deltaTime;

	// Randomly change the direction every FRAME_CHANGE_INTERVAL frames
	static std::mt19937 randomEngine(time(nullptr));
	static std::uniform_real_distribution<float> randRotate(-20.0f, 20.0f);
	if (++m_frames == FRAME_CHANGE_INTERVAL)
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
		m_frames = 0;
	}

	// If we bump into a wall - change direction
	if (collideWithlevel(levelData))
	{
		m_direction = glm::rotate(m_direction, randRotate(randomEngine));
	}
}
