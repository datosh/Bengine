#include "power_up.h"

const std::vector<std::string> PowerUp::m_powerUpNames{ "paddle_size", "speed_up", "times_two" };

PowerUp::PowerUp()
{
}

PowerUp::~PowerUp()
{
}

void PowerUp::init(MainGame * mainGame)
{
	std::cout << "PowerUp standard init called" << std::endl;
	init(glm::vec2(0, 0), glm::vec2(100, 50), Bengine::ColorRGBA8(255, 255, 255, 255), "speed_up");
}

void PowerUp::update(MainGame * mainGame, float deltaTime)
{
	m_position.y -= m_speed * deltaTime;

	if (m_position.y < -50) m_alive = false;
}

void PowerUp::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID;
	textureID = Bengine::ResourceManager::getTexture(m_textureName).id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, m_width, m_height);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, m_color);
}

void PowerUp::init(glm::vec2 position, glm::vec2 size, Bengine::ColorRGBA8 color, const std::string & name)
{
	m_alive = true;

	m_position = position;
	m_width = size.x;
	m_height = size.y;
	m_color = color;
	m_name = name;
	m_speed = 2.0f;

	m_textureName = "Textures/power_up_" + m_name + ".png";
}
