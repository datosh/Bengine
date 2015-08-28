#include "mainGame.h"

MainGame::MainGame() : 
	m_time(0), 
	m_screenWidth(1024),
	m_screenHeight(768), 
	m_gameState(GameState::PLAY),
	m_maxFPS(40),
	m_fps(0),
	m_frameTime(0)
{
	m_camera.init(m_screenWidth, m_screenHeight);
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();

	m_sprites.push_back(new Bengine::Sprite());
	m_sprites.back()->init(0.0f, 0.0f, m_screenWidth / 2.0f, m_screenHeight / 2.0f, "Textures/PNG/CharacterRight_Standing.png");

	m_sprites.push_back(new Bengine::Sprite());
	m_sprites.back()->init(m_screenWidth / 2.0f, 0.0f, m_screenWidth / 2.0f, m_screenHeight / 2.0f, "Textures/PNG/CharacterRight_Standing.png");
	
	
	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window.create("Game Engine", m_screenWidth, m_screenHeight, 0);

	initShaders();
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
		float startTicks = static_cast<float>(SDL_GetTicks());

		processInput();
		m_camera.update();
		drawGame();

		m_time += 0.12f;

		calculateFPS();

		// print only once every 10 frames
		static int frameCounter = 0;
		if (frameCounter++ == 10)
		{
			std::cout << m_fps << std::endl;
			frameCounter = 0;
		}

		// Limit FPS to max FPS
		float frameTicks = SDL_GetTicks() - startTicks;
		if ((1000.0f / m_maxFPS) > frameTicks)
		{
			SDL_Delay(static_cast<Uint32>((1000.0f / m_maxFPS) - frameTicks));
		}
	}
}

void MainGame::processInput()
{
	const float CAMERA_SPEED = 10.0f;
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
			//std::cout << event.motion.x << ":" << event.motion.y << std::endl;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_w:
				m_camera.setPositon(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
				break;
			case SDLK_s:
				m_camera.setPositon(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
				break;
			case SDLK_a:
				m_camera.setPositon(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
				break;
			case SDLK_d:
				m_camera.setPositon(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
				break;
			case SDLK_q:
				m_camera.setScale(m_camera.getScale() + SCALE_SPEED);
				break;
			case SDLK_e:
				m_camera.setScale(m_camera.getScale() + -SCALE_SPEED);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
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

	GLint timeLocation = m_colorProgram.getUniformLocation("time");
	glUniform1f(timeLocation, m_time);

	GLint pLocation = m_colorProgram.getUniformLocation("P");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	for (auto sprite : m_sprites)
	{
		sprite->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.unuse();

	m_window.swapBuffer();

}

void MainGame::calculateFPS()
{
	// Init once
	static int currentFrame = 0;
	static const int NUM_SAMPLES = 100;
	static float frameTimes[NUM_SAMPLES];
	static float prevTicks = static_cast<float>(SDL_GetTicks());
	
	// Get delta
	float currentTicks = static_cast<float>(SDL_GetTicks());
	m_frameTime = currentTicks - prevTicks;
	frameTimes[currentFrame++ % NUM_SAMPLES] = m_frameTime;
	prevTicks = currentTicks;

	int count;
	if (currentFrame < NUM_SAMPLES)
	{
		count = currentFrame;
	}
	else
	{
		count = NUM_SAMPLES;
	}

	float frameTimeAverage = 0;
	for (auto time : frameTimes)
	{
		frameTimeAverage += time;
	}
	frameTimeAverage /= count;

	if(frameTimeAverage > 0)
	{
		m_fps = 1000.0f / frameTimeAverage;
	}
	else
	{
		m_fps = 60.0f;
	}
}


