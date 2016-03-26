#include "MainGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>

#include "utility.h"
#include "../include/glm/gtx/rotate_vector.hpp"

#include <SDL.h>
#include <iostream>

MainGame::MainGame() :
	m_screenWidth(1024),
	m_screenHeight(768),
	m_fps(0),
	m_points(0)
{
	m_gameState = GameState::PLAY;
}

MainGame::~MainGame() {
	
}

void MainGame::run() {
	initSystems();
	initLevel();

	gameLoop();
}

void MainGame::game_over(std::string message)
{
	std::cout << message << std::endl;

	m_gameState = GameState::EXIT;
}

void MainGame::add_power_up(const std::string & name, glm::vec2 position)
{
	PowerUp* pu = new PowerUp();
	pu->init(position, glm::vec2(50, 25), Bengine::ColorRGBA8(105, 55, 0, 255), name);
	m_powerUps.push_back(pu);
}

void MainGame::add_random_power_up(glm::vec2 position)
{
	auto listOfPowerUps = PowerUp::get_powerUpNames();
	auto randIdx = util::randInt(0, listOfPowerUps.size() - 1);
	add_power_up(listOfPowerUps[randIdx], position);
}

void MainGame::double_balls()
{
	size_t num_balls = m_balls.size();
	for (size_t i = 0; i < num_balls; ++i)
	{
		auto& ball = m_balls[i];

		auto velo = ball->get_velocity();
		auto pos = ball->get_position();

		velo = glm::rotate(velo, 20.0f);
		
		m_balls.push_back(new Ball());
		m_balls.back()->init(this);
		m_balls.back()->set_velocity(velo);
		m_balls.back()->set_position(pos);
	}
}

void MainGame::initSystems() {
	// Init GameEngine
	Bengine::init();

	m_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.03f, 0.27f, 0.36f, 1.0f);

	initShaders();

	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Init sprite font
	m_spriteFont = new Bengine::SpriteFont("Fonts/chintzy.ttf", 64);

	m_camera.init(m_screenWidth, m_screenHeight);
	// Set the bottom left corner to (0,0)
	m_camera.setPositon(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPositon(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));
}

void MainGame::initLevel()
{
	// Do all init stuff here
	
	// Init the player/paddle
	m_paddle = new Paddle();
	m_paddle->init(this);
	m_paddle->setInputManager(&m_inputManager);

	// Init the ball
	m_balls.push_back(new Ball());
	m_balls.back()->init(this);

	// Init the bricks
	Bengine::ColorRGBA8 brickBaseColor(2, 29, 39, 255);
	glm::vec2 brickBaseSize(75, 30);
	std::string brickTexturePath("Textures/light_bricks.png");
	for (size_t i = 0; i < 10; ++i)
	{
		m_bricks.push_back(new Brick());
		m_bricks.back()->init(glm::vec2(100 + i * 80, 450), brickBaseSize, brickBaseColor, 7);
	}
}

void MainGame::initShaders() {
	// Compile our color shader
	m_textureProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	const float DESIRED_FPS = 60.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	const int MAX_PHYSICS_STEPS = 6;

	Bengine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(DESIRED_FPS);

	const float CAMERA_SCALE = 1.0f / 1.0f;
	m_camera.setScale(CAMERA_SCALE);

	float previousTicks = SDL_GetTicks();

	while (m_gameState == GameState::PLAY)
	{
		fpsLimiter.begin();

		// Calculate the delta time
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		m_inputManager.update();
		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			// BEGIN UPDATES
			
			m_paddle->update(this, deltaTime);
			for (auto ball : m_balls) {
				ball->update(this, deltaTime);
			}
			for (auto brick : m_bricks) {
				brick->update(this, deltaTime);
			}
			for (auto pu : m_powerUps) {	
				pu->update(this, deltaTime);
			}

			// Remove all dead bricks
			std::for_each(m_bricks.begin(), m_bricks.end(), DeleteIfBrickDead());
			m_bricks.erase(std::remove(m_bricks.begin(), m_bricks.end(), nullptr), m_bricks.end());
			// If no more bricks => game over, since won
			if (m_bricks.size() == 0) game_over("No more bricks - yeahy!\n");

			// Remove all dead power ups
			std::for_each(m_powerUps.begin(), m_powerUps.end(), DeleteIfPowerUpDead());
			m_powerUps.erase(std::remove(m_powerUps.begin(), m_powerUps.end(), nullptr), m_powerUps.end());

			// Remoave all dead balls
			std::for_each(m_balls.begin(), m_balls.end(), DeleteIfBallDead());
			m_balls.erase(std::remove(m_balls.begin(), m_balls.end(), nullptr), m_balls.end());
			// If no more balls left => game over
			if (m_balls.size() == 0) game_over("Out of balls!\n");

			// END UPDATES

			totalDeltaTime -= deltaTime;
			++i;
		}

		m_camera.update();

		m_hudCamera.update();

		drawGame();

		m_fps = fpsLimiter.end();
		// std::cout << m_fps << std::endl;
	}

}

void MainGame::processInput() {
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the texture uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Start drawing on agent batch
	m_agentSpriteBatch.begin();

	// DRAW HERE
	m_paddle->draw(m_agentSpriteBatch);
	for (auto ball : m_balls) {
		ball->draw(m_agentSpriteBatch);
	}
	for (auto brick : m_bricks) {
		brick->draw(m_agentSpriteBatch);
	}
	for (auto pu : m_powerUps) {
		pu->draw(m_agentSpriteBatch);
	}

	// TIL HERE

	// End drawing on agent batch
	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	// Render the HUD
	drawHUD();

	m_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();
}

void MainGame::drawHUD()
{
	// TOOD: Make this save
	char buffer[256];

	// Use the hud camera for drawing the hud
	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	// Start rendering the HUD

	sprintf_s(buffer, "Points %u", m_points);
	m_spriteFont->draw(m_hudSpriteBatch, buffer,
		glm::vec2(m_screenWidth / 2, m_screenHeight - 50),
		glm::vec2(.5f), 0.0f,
		Bengine::ColorRGBA8(255, 255, 255, 255));
	
	// End rendering the HUD

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}
