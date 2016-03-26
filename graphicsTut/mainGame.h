#pragma once

#include <Bengine/Window.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/InputManager.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/SpriteFont.h>
#include <Bengine/AudioEngine.h>
#include <Bengine/ParticleEngine2D.h>
#include <Bengine/ParticleBatch2D.h>

#include "paddle.h"
#include "ball.h"
#include "brick.h"
#include "power_up.h"

enum class GameState {
	PLAY,
	EXIT
};

class MainGame
{
public:
	MainGame();
	~MainGame();

	/// Runs the game
	void run();

	// GETTER
	int get_width() const { return m_screenWidth; }
	int get_height() const { return m_screenHeight; }
	Paddle* get_paddle() const { return m_paddle; }
	Bengine::Camera2D get_mainCamera() const { return m_camera; }
	std::vector<Brick*> get_bricks() const { return m_bricks; }
	std::vector<PowerUp*> get_powerUps() const { return m_powerUps; }
	float get_powerUpChance() const { return 1.0f; }

	/// So any object can signalize that the game is over
	void game_over(std::string message);
	/// Add points to the score
	void add_points(unsigned int p) { m_points += p; }
	/// Spawn a specific powerup in the world
	void add_power_up(const std::string & name, glm::vec2 position);
	/// Spawn a random powerup in the world
	void add_random_power_up(glm::vec2 position);
	/// Spawns a new ball for every existing ball
	void double_balls();

private:
	/// Initializes the core systems
	void initSystems();
	void initLevel();

	/// Initializes the shaders
	void initShaders();

	/// Main game loop for the program
	void gameLoop();

	/// Handles input processing
	void processInput();

	/// Renders the game
	void drawGame();

	/// Drawrs the HUD
	void drawHUD();

	/// Member Variables
	Bengine::Window m_window; ///< The game window

	Bengine::GLSLProgram m_textureProgram; ///< The shader program

	Bengine::InputManager m_inputManager; ///< Handles input

	Bengine::Camera2D m_camera; ///< Main Camera
	Bengine::Camera2D m_hudCamera; ///< HUD Camera

	Bengine::SpriteBatch m_agentSpriteBatch;
	Bengine::SpriteBatch m_hudSpriteBatch;

	int m_screenWidth, m_screenHeight;

	float m_fps;

	Bengine::SpriteFont* m_spriteFont;

	GameState m_gameState;


	// NEW OBJECTS
	Paddle* m_paddle;
	std::vector<Ball*> m_balls;
	std::vector<Brick*> m_bricks;
	std::vector<PowerUp*> m_powerUps;

	unsigned int m_points;
};

