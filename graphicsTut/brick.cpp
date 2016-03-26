#include "brick.h"

Brick::Brick()
{
}

Brick::~Brick()
{
}

void Brick::init(MainGame * mainGame)
{
	std::cout << "Brick standard init called" << std::endl;
	init(glm::vec2(0, 0), glm::vec2(100, 50), Bengine::ColorRGBA8(255, 255, 255, 255));
}

void Brick::update(MainGame * mainGame, float deltaTime)
{
}

void Brick::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID;
	textureID = Bengine::ResourceManager::getTexture("Textures/brick.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, m_width, m_height);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, m_color);
}

void Brick::init(glm::vec2 position, glm::vec2 size, Bengine::ColorRGBA8 color, unsigned int points)
{
	m_alive = true;
	m_points = points;

	m_position = position;
	m_width = size.x;
	m_height = size.y;
	m_color = color;
}

void Brick::kill()
{
	m_alive = false;
}

