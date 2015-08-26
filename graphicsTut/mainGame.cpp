#include "mainGame.h"

MainGame::MainGame() : m_time(0), m_screenWidth(1024), m_screenHeight(768), m_window(nullptr), m_gameState(GameState::PLAY)
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();


	m_sprites.push_back(new Sprite());
	m_sprites.back()->init(-1.0f, -1.0f, 1.0f, 1.0f, "Textures/PNG/CharacterRight_Standing.png");

	m_sprites.push_back(new Sprite());
	m_sprites.back()->init(0.0f, -1.0f, 1.0f, 1.0f, "Textures/PNG/CharacterRight_Standing.png");
	
	
	gameLoop();
}

void MainGame::initSystems()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	m_window = SDL_CreateWindow("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		m_screenWidth, m_screenHeight, SDL_WINDOW_OPENGL);
	if (!m_window)
	{
		fatalError("SDL_CreateWindow failed!");
	}

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
	if (!glContext)
	{
		fatalError("SDL_GL_CreateContext failed!");
	}

	glewExperimental = true;
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		fatalError("glewInit failed!");
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

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
		processInput();
		drawGame();
		m_time += 0.21f;

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
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
			//std::cout << event.motion.x << ":" << event.motion.y << std::endl;
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

	for (auto sprite : m_sprites)
	{
		sprite->draw();
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.unuse();

	SDL_GL_SwapWindow(m_window);

}


