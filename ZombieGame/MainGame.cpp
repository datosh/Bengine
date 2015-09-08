#include "mainGame.h"

MainGame::MainGame() :
	m_time(0),
	m_screenWidth(800),
	m_screenHeight(600),
	m_gameState(GameState::PLAY),
	m_maxFPS(40),
	m_fps(0)
{
	m_camera.init(m_screenWidth, m_screenHeight);

	// Create Player
	glm::vec2 playerPos(0.0f);
	glm::vec2 playerDir(0.0f);
	Bengine::Color playerColor = { 255, 0, 0, 255 };
	m_player = new Player(playerPos, playerDir, CAMERA_SPEED, 15.0f, playerColor);

	// Create Zombies
	Bengine::Color zombieColor = { 0, 255, 0, 255 };
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> widthDis(-m_screenWidth / 2, m_screenWidth / 2);
	std::uniform_real_distribution<> heightDis(-m_screenHeight / 2, m_screenHeight / 2);
	std::uniform_real_distribution<> dirDis(0, 1);
	for (auto i = 0; i < 10; ++i)
	{
		glm::vec2 zombiePos(widthDis(gen), heightDis(gen));
		glm::vec2 zombieDir(dirDis(gen), dirDis(gen));
		m_zombies.emplace_back(zombiePos, zombieDir, CAMERA_SPEED * 0.7f, 10.0f, zombieColor);
		m_zombies.back().follow(m_player);
	}
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

	m_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);

	initShaders();

	m_spriteBatch.init();
	m_fpsLimiter.init(m_maxFPS);
}

void MainGame::initShaders()
{
	m_colorProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
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
		m_camera.update();
		tick();
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
		glm::vec2 delta(0.0f, CAMERA_SPEED);
		m_player->moveRel(delta);
	}
	if (m_inputManager.isKeyPressed(SDLK_s))
	{
		glm::vec2 delta(0.0f, -CAMERA_SPEED);
		m_player->moveRel(delta);
	}
	if (m_inputManager.isKeyPressed(SDLK_a))
	{
		glm::vec2 delta(-CAMERA_SPEED, 0.0f);
		m_player->moveRel(delta);
	}
	if (m_inputManager.isKeyPressed(SDLK_d))
	{
		glm::vec2 delta(CAMERA_SPEED, 0.0f);
		m_player->moveRel(delta);
	}
	if (m_inputManager.isKeyPressed(SDLK_q))
	{
	}
	if (m_inputManager.isKeyPressed(SDLK_e))
	{
	}
	if (m_inputManager.isKeyPressed(SDL_BUTTON_LEFT) ||
		m_inputManager.isKeyPressed(SDLK_SPACE))
	{
		static Uint32 lastBullet = SDL_GetTicks();
		const Uint32 bulletDelay = 500;

		if (SDL_GetTicks() - lastBullet > 500)
		{
			lastBullet = SDL_GetTicks();

			glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
			glm::vec2 worldCoords = m_camera.convertScreenToWorld(mouseCoords);

			glm::vec2 playerPositon = m_player->getPos();
			glm::vec2 direction = worldCoords - playerPositon;
			direction = glm::normalize(direction);

			Bengine::Color bulletColor = { 52, 52, 52, 255 };

			m_bullets.emplace_back(playerPositon, direction, 1.7f * CAMERA_SPEED, 3.0f, bulletColor);
		}
		
	}
}

void MainGame::drawGame()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);

	GLint textureLoaction = m_colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLoaction, 0);

	GLint pLocation = m_colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	// Draw all sprites via batch
	m_spriteBatch.begin();
	draw();
	m_spriteBatch.end();
	m_spriteBatch.renderBatch();

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.unuse();

	m_window.swapBuffer();
}

void MainGame::tick()
{
	m_player->update();

	for (auto & zombie : m_zombies)
	{
		zombie.update();
		if (zombie.overlaps(*m_player))
		{
			std::cout << "COLLISION" << std::endl;
		}
	}

	for (auto & bullet : m_bullets)
	{
		bullet.update();
		
		for (auto & zombie : m_zombies)
		{
			if (bullet.alive() && zombie.alive() && bullet.overlaps(zombie))
			{
				zombie.die();
				bullet.die();
			}
		}
	}

	std::cout << m_bullets.size() << std::endl;

	// Get rid of dead entities
	m_bullets.erase(std::remove_if(m_bullets.begin(), m_bullets.end(), [](Bullet b) {return !b.alive();}), m_bullets.end());
	m_zombies.erase(std::remove_if(m_zombies.begin(), m_zombies.end(), [](Zombie z) {return !z.alive();}), m_zombies.end());
}

void MainGame::draw()
{
	m_player->draw(m_spriteBatch);

	for (auto zombie : m_zombies)
	{
		zombie.draw(m_spriteBatch);
	}

	for (auto bullet : m_bullets)
	{
		bullet.draw(m_spriteBatch);
	}
}



