#include "Window.h"

namespace Bengine {

	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags)
	{
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE) { flags |= SDL_WINDOW_HIDDEN; }
		if (currentFlags & FULLSCREEN) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }
		if (currentFlags & BORDERLESS) { flags |= SDL_WINDOW_BORDERLESS; }

		m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screenWidth, screenHeight, flags);
		if (!m_sdlWindow)
		{
			fatalError("SDL_CreateWindow failed!");
		}

		SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
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

		// Check OpenGL Version
		std::printf("*** OpenGL Version: %s ***\n", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

		// Enable VSync
		SDL_GL_SetSwapInterval(1);

		// Enable alpha blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		return 0;
	}

	void Window::swapBuffer()
	{
		SDL_GL_SwapWindow(m_sdlWindow);
	}
}