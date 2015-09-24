#include "Zombie.h"
#include "Human.h"


Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::init(float speed, glm::vec2 pos)
{
	m_speed = speed;
	m_position = pos;
	m_color = Bengine::ColorRGBA8(0, 160, 0, 255);

	m_health = 150;
}

void Zombie::update(const std::vector<std::string>& levelData,
	std::vector<Human*>& humans,
	std::vector<Zombie*>& zombies,
	float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr)
	{
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += direction * m_speed * deltaTime;
	}

	collideWithlevel(levelData);
}

Human * Zombie::getNearestHuman(std::vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	// Brakets are necessary so windows.h's max() isn't used
	float smallestDistance = (std::numeric_limits<float>::max)();

	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}

	return closestHuman;
}
