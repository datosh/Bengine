#pragma once

#include "stdafx.h"

#include <glm.hpp>
#include <gtx\rotate_vector.hpp>

#include "Bullet.h"


class Gun
{
public:
	Gun(std::string name, int fireRate, int bulletsPerShot, float spread, float bulletDamage, float bulletSpeed);
	~Gun();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Bullet>& bullets);

private:

	void fire(const glm::vec2& direction, const glm::vec2& position, std::vector<Bullet>& bullets);

	std::string m_name;
	int m_fireRate; ///< Fire rate in terms of frames
	int m_bulletsPerShot; ///< How many bullets are fired at a time
	float m_spread; ///< Accuracy

	float m_bulletSpeed;
	float m_bulletDamage;

	int m_frameCounter;
};

