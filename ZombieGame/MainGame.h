#pragma once

#include "stdafx.h"

#include "Entity.h"
#include "Player.h"
#include "Zombie.h"

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
#include <Bengine\SpriteBatch.h>
#include <Bengine\InputManager.h>
#include <Bengine\Timing.h>


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
	void tick();
	void draw();

	const float CAMERA_SPEED = 3.0f;
	const float SCALE_SPEED = 0.1f;

	int m_screenWidth, m_screenHeight;
	float m_time;
	float m_fps, m_maxFPS;

	GameState m_gameState;

	Bengine::Window m_window;
	Bengine::GLSLProgram m_colorProgram;
	Bengine::Camera2D m_camera;
	Bengine::SpriteBatch m_spriteBatch;
	Bengine::InputManager m_inputManager;
	Bengine::FPSLimiter m_fpsLimiter;

	Player* m_player;
	std::vector<Zombie> m_zombies;
};
