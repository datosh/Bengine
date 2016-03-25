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

#include "Player.h"
#include "Level.h"
#include "Zombie.h"
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

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

private:
	/// Initializes the core systems
	void initSystems();
	void initLevel();

	/// Initializes the shaders
	void initShaders();

	/// Main game loop for the program
	void gameLoop();

	void updateAgents(float deltaTime);
	void updateBullets(float deltaTime);

	/// Handles input processing
	void processInput();

	/// Renders the game
	void drawGame();

	/// Drawrs the HUD
	void drawHUD();

	/// Adds blood tot he particle engine
	void addBlood(const glm::vec2& position, int numParticles);

	/// Member Variables
	Bengine::Window m_window; ///< The game window

	Bengine::GLSLProgram m_textureProgram; ///< The shader program

	Bengine::InputManager m_inputManager; ///< Handles input

	Bengine::Camera2D m_camera; ///< Main Camera
	Bengine::Camera2D m_hudCamera; ///< HUD Camera

	Bengine::SpriteBatch m_agentSpriteBatch;
	Bengine::SpriteBatch m_hudSpriteBatch;

	Bengine::AudioEngine m_audioEngine;

	Bengine::ParticleEngine2D m_particleEngine;
	Bengine::ParticleBatch2D* m_bloodParticleBatch;

	std::vector<Level*> m_levels; ///< Vector of all levels

	int m_screenWidth, m_screenHeight;

	float m_fps;

	int m_currentLevel;

	Player* m_player;
	std::vector<Human*> m_humans;
	std::vector<Zombie*> m_zombies;
	std::vector<Bullet> m_bullets;

	Bengine::SpriteFont* m_spriteFont;

	GameState m_gameState;
};

