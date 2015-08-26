#pragma once

#include "stdafx.h"

#include "Errors.h"
#include "sprite.h"
#include "GLSLProgram.h"
#include "GLTexture.h"
#include "ImageLoader.h"

#include <SDL.h>
#include <glew.h>

enum class GameState {
	PLAY,
	EXIT,
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	int m_screenWidth, m_screenHeight;
	float m_time;
	SDL_Window* m_window;
	GameState m_gameState;
	Sprite m_sprite;
	GLTexture m_playerTexture;
	GLSLProgram m_colorProgram;
};
