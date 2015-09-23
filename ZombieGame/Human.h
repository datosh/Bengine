#pragma once

#include "stdafx.h"
#include "Agent.h"
#include <gtx\rotate_vector.hpp>

const int FRAME_CHANGE_INTERVAL = 120;

class Human : public Agent
{
public:
	Human();
	virtual ~Human();

	void init(float speed, glm::vec2 pos);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Human*>& humans,
		std::vector<Zombie*>& zombies);

private:
	glm::vec2 m_direction;
	int m_frames;
};

