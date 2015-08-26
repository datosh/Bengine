#pragma once

#include "stdafx.h"

#include "Errors.h"

#include <SDL.h>
#include <glew.h>

namespace Bengine {
	enum WindowFlags {
		INVISIBLE = 0x01,
		FULLSCREEN = 0x02,
		BORDERLESS = 0x04,
	};

	class Window
	{
	public:
		Window();
		~Window();

		int create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags);

		void swapBuffer();

		int getScreenWidth() { return m_screenWidth; }
		int getScreenHeight() { return m_screenHeight; }

	private:
		int m_screenWidth, m_screenHeight;
		SDL_Window* m_sdlWindow;
	};
}