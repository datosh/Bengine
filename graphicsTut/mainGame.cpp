#include "MainGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>

#include <SDL.h>
#include <iostream>

MainGame::MainGame() :
	m_screenWidth(1024),
	m_screenHeight(768),
	m_fps(0)
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

void MainGame::initSystems() {
	// Init GameEngine
	Bengine::init();

	m_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

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
	m_paddle = new Paddle();
	m_paddle->init(this);
	m_paddle->setInputManager(&m_inputManager);

	m_ball = new Ball();
	m_ball->init(this);
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
			m_ball->update(this, deltaTime);

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
	m_ball->draw(m_agentSpriteBatch);

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

	sprintf_s(buffer, "Points ");
	m_spriteFont->draw(m_hudSpriteBatch, buffer,
		glm::vec2(m_screenWidth / 2, m_screenHeight - 50),
		glm::vec2(.5f), 0.0f,
		Bengine::ColorRGBA8(255, 255, 255, 255));
	
	// End rendering the HUD

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}
