#pragma once

#include "stdafx.h"

#include <SDL.h>
#include <glew.h>
#include <glm.hpp>

#include <Bengine\Bengine.h>
#include <Bengine\Errors.h>
#include <Bengine\sprite.h>
#include <Bengine\GLSLProgram.h>
#include <Bengine\GLTexture.h>
#include <Bengine\Window.h>
#include <Bengine\Camera2D.h>


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
	void calculateFPS();

	int m_screenWidth, m_screenHeight;
	float m_time;
	float m_fps, m_frameTime;
	float m_maxFPS;
	Bengine::Window m_window;
	GameState m_gameState;
	std::vector<Bengine::Sprite*> m_sprites;
	Bengine::GLSLProgram m_colorProgram;
	Bengine::Camera2D m_camera;
};
