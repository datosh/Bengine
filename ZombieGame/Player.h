#pragma once

#include "Human.h"
#include "Bullet.h"
#include <Bengine\InputManager.h>
#include <Bengine\Camera2D.h>


class Gun;

class Player : public Human
{
public:
	Player();
	~Player();

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies,
		float deltaTime) override;

	void addGun(Gun* gun);

	void init(float speed, glm::vec2 position, 
		Bengine::InputManager* inputManager,
		Bengine::Camera2D* camera,
		std::vector<Bullet>* bullets);

private:
	Bengine::InputManager* m_inputManager;
	Bengine::Camera2D* m_camera;
	std::vector<Gun*> m_guns;
	std::vector<Bullet>* m_bullets;
	int m_currentGunIndex;
};

