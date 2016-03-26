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
	m_last_pause_pressed = util::get_time();
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
		// Cast to ball
		Ball* ball = dynamic_cast<Ball*>(m_balls[i]);

		// Get velo and position
		auto velo = ball->get_velocity();
		auto pos = ball->get_position();

		// Alter the direction vector slightly
		velo = glm::rotate(velo, 20.0f);
		
		// Create new ball and put in vector
		Ball* new_ball = new Ball();
		new_ball->init(this);
		new_ball->set_velocity(velo);
		new_ball->set_position(pos);
		m_balls.push_back(new_ball);
	}
}

void MainGame::spawn_brick_paricles(glm::vec2 position)
{
	// Spawn 4x2 brick particles that fly away in different directions
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 2; ++j) {
			auto bp = new BrickParticle();
			bp->init(this);
			bp->set_position(position + glm::vec2(25 * i, 25 * j));
			m_brickParticles.push_back(bp);
		}
	}
}

void MainGame::erase_dead_entities(std::vector<Entity*>& entities)
{
	std::for_each(entities.begin(), entities.end(), DeleteIfEntityDead());
	entities.erase(std::remove(entities.begin(), entities.end(), nullptr), entities.end());
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
	for (size_t i = 0; i < 10; ++i)
	{
		Brick* new_brick = new Brick();
		new_brick->init(glm::vec2(100 + i * 80, 450), brickBaseSize, brickBaseColor, 7);
		m_bricks.push_back(new_brick);
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

	while (m_gameState != GameState::EXIT)
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
			if (m_inputManager.isKeyDown(SDLK_p) && util::time_dif_gt(m_last_pause_pressed, util::get_time(), m_time_between_pause)) {

				m_last_pause_pressed = util::get_time();

				if (m_gameState == GameState::PAUSED) m_gameState = GameState::PLAY;
				else m_gameState = GameState::PAUSED;
			}
			
			// Only update the actual game when the game is not paused
			if (m_gameState != GameState::PAUSED) {
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
				for (auto bp : m_brickParticles) {
					bp->update(this, deltaTime);
				}

				// Remove all dead bricks
				erase_dead_entities(m_bricks);
				// If no more bricks => game over, since won
				if (m_bricks.size() == 0) game_over("No more bricks - yeahy!\n");

				// Remove all dead power ups
				erase_dead_entities(m_powerUps);

				// Remoave all dead balls
				erase_dead_entities(m_balls);
				// If no more balls left => game over
				if (m_balls.size() == 0) game_over("Out of balls!\n");

				// Remove all dead brick paricles
				erase_dead_entities(m_brickParticles);

				
			}
			else // If the game is paused
			{
				// Make the paused message animate
				m_paused_alpha -= m_alpha_anim_speed;
				if (m_paused_alpha <= 0) m_paused_alpha = 255;
			}

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
	for (auto bp : m_brickParticles) {
		bp->draw(m_agentSpriteBatch);
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

	// If game is paused show PAUSED message
	if (m_gameState == GameState::PAUSED) {
		m_spriteFont->draw(m_hudSpriteBatch, m_paused_msg.c_str(),
			glm::vec2(m_screenWidth / 2 - m_paused_msg.length() * 2, m_screenHeight / 2 + 5),
			glm::vec2(.5f), 0.0f,
			Bengine::ColorRGBA8(255, 255, 255, m_paused_alpha));
	}
	
	// End rendering the HUD

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}
