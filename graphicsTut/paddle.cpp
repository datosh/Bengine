#include "paddle.h"
#include "mainGame.h"


Paddle::Paddle()
{
}

Paddle::~Paddle()
{
}

void Paddle::init(MainGame* mainGame)
{
	m_speed = 6.5f;
	m_width = 150;
	m_height = 25;

	m_position.x = -m_width / 2 + mainGame->get_width() / 2;
	m_position.y = 0;
}

void Paddle::update(MainGame* mainGame, float deltaTime)
{
	auto coords = m_inputManager->getMouseCoords();
	auto transcoords = mainGame->get_mainCamera().convertScreenToWorld(coords);
	
	// Handle movement
	if (m_inputManager->isKeyDown(SDLK_d)) {
		m_position.x += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_a)) {
		m_position.x -= m_speed * deltaTime;
	}
	// Keep paddle inside the screen
	// Screen is in range [-screenwidth;screenwidth]
	if (m_position.x < 0)
		m_position.x = 0;
	if (m_position.x > mainGame->get_width() - m_width) 
		m_position.x = mainGame->get_width() - m_width;
}

void Paddle::draw(Bengine::SpriteBatch & spriteBatch)
{
	static int textureID = Bengine::ResourceManager::getTexture("Textures/PNG/LandPiece_LightGray.png").id;
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(m_position.x, m_position.y, m_width, m_height);
	spriteBatch.draw(destRect, uvRect, textureID, 0.0f, Bengine::ColorRGBA8(255, 255, 255, 255));
}
