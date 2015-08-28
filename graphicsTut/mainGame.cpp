#include "mainGame.h"

MainGame::MainGame() : 
	m_time(0), 
	m_screenWidth(1024),
	m_screenHeight(768), 
	m_gameState(GameState::PLAY),
	m_maxFPS(40),
	m_fps(0)
{
	m_camera.init(m_screenWidth, m_screenHeight);
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
 	
	gameLoop();
}

void MainGame::initSystems()
{
	Bengine::init();

	m_window.create("Game Engine", m_screenWidth, m_screenHeight, 0);

	initShaders();

	m_spriteBatch.init();
	m_fpsLimiter.init(m_maxFPS);
}

void MainGame::initShaders()
{
	m_colorProgram.compileShaders("Shaders/colorShading.vert", "Shaders/colorShading.frag");
	m_colorProgram.addAttribute("vertexPosition");
	m_colorProgram.addAttribute("vertexColor");
	m_colorProgram.addAttribute("vertexUV");
	m_colorProgram.linkShaders();
}

void MainGame::gameLoop()
{
	while (m_gameState != GameState::EXIT)
	{
		// Start new frame
		m_fpsLimiter.begin();
		
		// Do a tick
		processInput();
		m_time += 0.12f;
		m_camera.update();
		drawGame();

		// Check for potential errors
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "OpenGL error: " << err << std::endl;
		}
		
		m_fps = m_fpsLimiter.end();

		// print only once every 10 frames
		static int frameCounter = 0;
		if (frameCounter++ == 10000)
		{
			std::cout << m_fps << std::endl;
			frameCounter = 0;
		}
	}
}

void MainGame::processInput()
{
	const float CAMERA_SPEED = 3.0f;
	const float SCALE_SPEED = 0.1f;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(event.button.button);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(event.key.keysym.sym);
			break;
		default:
			break;
		}
	}

	if (m_inputManager.isKeyPressed(SDLK_w))
	{
		m_camera.setPositon(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (m_inputManager.isKeyPressed(SDLK_s))
	{
		m_camera.setPositon(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (m_inputManager.isKeyPressed(SDLK_a))
	{
		m_camera.setPositon(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (m_inputManager.isKeyPressed(SDLK_d))
	{
		m_camera.setPositon(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (m_inputManager.isKeyPressed(SDLK_q))
	{
		m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
	}
	if (m_inputManager.isKeyPressed(SDLK_e))
	{
		m_camera.setScale(m_camera.getScale() + -SCALE_SPEED);
	}
	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
	}
}

void MainGame::drawGame()
{
	// TODO: Figure out what this does
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLoaction = m_colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLoaction, 0);

	GLint pLocation = m_colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	m_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static Bengine::GLTexture texture = Bengine::ResourceManager::getTexture("Textures/PNG/CharacterRight_Walk2.png");
	Bengine::Color color = { 255, 255, 255, 255 };
	m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.unuse();

	m_window.swapBuffer();

}




