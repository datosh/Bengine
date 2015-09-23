#include "Level.h"

#include <Bengine\Errors.h>

Level::Level(const std::string& fileName)
{
	std::ifstream file;
	file.open(fileName);

	if (file.fail()) {
		Bengine::fatalError("Failed to open " + fileName);
	}

	// Throw away the first string in tmp and only save number of humans
	std::string tmp;
	file >> tmp >> m_numHumans;

	std::getline(file, tmp); // Throw away the rest of the first line
	while (std::getline(file, tmp)) {
		m_levelData.push_back(tmp);
	}

	m_spriteBatch.init();
	m_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	for (int y = 0; y < m_levelData.size(); ++y)
	{
		for (int x = 0; x < m_levelData[y].size(); ++x)
		{
			// Grab the tile
			char tile = m_levelData[y][x];

			// Get dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			// Process the tile
			switch (tile)
			{
			case 'B':
			case 'R':
				m_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/red_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case 'G':
				m_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/light_bricks.png").id,
					0.0f,
					whiteColor);
				break;
			case 'L':
				m_spriteBatch.draw(destRect,
					uvRect,
					Bengine::ResourceManager::getTexture("Textures/glass.png").id,
					0.0f,
					whiteColor);
				break;
			case '@':
				m_levelData[y][x] = '.';
				m_startPlayerPos.x = x * TILE_WIDTH;
				m_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				m_levelData[y][x] = '.';
				m_zombieStartPos.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::cout << "Unexpected symbol " << tile << " at " << x << ":" << y;
				break;
			}
		}
	}

	m_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw()
{
	m_spriteBatch.renderBatch();
}
