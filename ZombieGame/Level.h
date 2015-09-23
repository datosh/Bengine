#pragma once

#include "stdafx.h"
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>

const INT TILE_WIDTH = 64;

class Level
{
public:
	// Load the level
	Level(const std::string& fileName);
	~Level();

	void draw();

	// Getters
	int getNumHumans() const { return m_numHumans; }
	int getWidth() const { return m_levelData[0].size(); }
	int getHeight() const { return m_levelData.size(); }
	const std::vector<std::string>& getLevelData() const { return m_levelData; }
	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const std::vector<glm::vec2>& getZombieStartPos() const { return m_zombieStartPos; }

private:
	std::vector<std::string> m_levelData;
	int m_numHumans;
	Bengine::SpriteBatch m_spriteBatch;

	glm::vec2 m_startPlayerPos;
	std::vector<glm::vec2> m_zombieStartPos;
};

