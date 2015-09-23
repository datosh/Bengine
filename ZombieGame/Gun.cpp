#include "Gun.h"

Gun::Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed) :
	m_name(name),
	m_fireRate(fireRate),
	m_bulletsPerShot(bulletsPerShot),
	m_spread(spread),
	m_bulletSpeed(bulletSpeed),
	m_bulletDamage(bulletDamage),
	m_frameCounter(0)
{
}

Gun::~Gun()
{
}

void Gun::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets)
{
	if (++m_frameCounter >= m_fireRate && isMouseDown)
	{
		fire(direction, position, bullets);
		m_frameCounter = 0;
	}
}

void Gun::fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets)
{
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_real_distribution<float> randSpread(-m_spread, m_spread);

	for (int i = 0; i < m_bulletsPerShot; ++i)
	{
		bullets.emplace_back(position, 
			glm::rotate(direction, randSpread(randomEngine)), 
			m_bulletDamage, 
			m_bulletSpeed);
	}
}
