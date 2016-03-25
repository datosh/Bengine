#include "MainGame.h"

#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>

#include <SDL.h>
#include <iostream>

MainGame::MainGame() :
	m_screenWidth(1024),
	m_screenHeight(768),
	m_fps(0),
	m_player(nullptr)
{
	m_gameState = GameState::PLAY;
}

MainGame::~MainGame() {
	for (auto l : m_levels)
	{
		delete l;
	}
	for (auto h : m_humans)
	{
		delete h;
	}
	for (auto z : m_zombies)
	{
		delete z;
	}
}

void MainGame::run() {
	initSystems();
	initLevel();

	Bengine::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play();	

	gameLoop();
}

void updateBloodParticle(Bengine::Particle2D& particle, float deltaTime)
{
	particle.pos += particle.vel * deltaTime;
	particle.color.a = static_cast<GLubyte>(particle.lifeTime * 255.0f);
}

void MainGame::initSystems() {
	// Init GameEngine
	Bengine::init();

	// Init sound must happen after Bengine::init
	m_audioEngine.init();

	m_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

	initShaders();

	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Init sprite font
	m_spriteFont = new Bengine::SpriteFont("Fonts/chintzy.ttf", 64);

	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPositon(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	// Init particles
	m_bloodParticleBatch = new Bengine::ParticleBatch2D();
	const float DECAY_RATE = 0.05f;
	m_bloodParticleBatch->init(1000, DECAY_RATE, 
		Bengine::ResourceManager::getTexture("Textures/particle.png"), 
		updateBloodParticle);
	m_particleEngine.addParticlebatch(m_bloodParticleBatch);
}

void MainGame::initLevel()
{
	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);

	m_humans.push_back(m_player);

	// Generate random humans
	std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);


	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); ++i)
	{
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		m_humans.back()->init(HUMAN_SPEED, pos);
	}

	// Generate zombies
	const std::vector<glm::vec2>& zombiePosition = m_levels[m_currentLevel]->getZombieStartPos();
	for (int i = 0; i < zombiePosition.size(); ++i)
	{
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_SPEED, zombiePosition[i]);
	}

	// Set up the players guns
	const float BULLET_SPEED = 10.0f;
	m_player->addGun(new Gun("Magnum", 30, 1, .05f, 30, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	m_player->addGun(new Gun("Shotgun", 60, 20, .4f, 3, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	m_player->addGun(new Gun("MP5", 5, 1, .1f, 12, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));
}

void MainGame::initShaders() {
	// Compile our color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}

void MainGame::gameLoop() {
	
	const float DESIRED_FPS = 60.0f;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	const int MAX_PHYSICS_STEPS = 6;

	Bengine::FPSLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(DESIRED_FPS);

	const float CAMERA_SCALE = 1.0f / 4.0f;
	m_camera.setScale(CAMERA_SCALE);

	float previousTicks = SDL_GetTicks();

	while (m_gameState == GameState::PLAY)
	{
		fpsLimiter.begin();

		// Calculate the delta time
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		m_inputManager.update();
		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
		{
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

			// BEGIN UPDATES
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(deltaTime);
			// END UPDATES

			totalDeltaTime -= deltaTime;
			++i;
		}

		m_camera.setPositon(m_player->getPosition());
		m_camera.update();

		m_hudCamera.update();

		drawGame();

		m_fps = fpsLimiter.end();
		std::cout << m_fps << std::endl;
	}

}

void MainGame::updateAgents(float deltaTime)
{
	// Update all the humans
	for (auto h : m_humans)
	{
		h->update(m_levels[m_currentLevel]->getLevelData(),
			m_humans, m_zombies, deltaTime);
	}

	// Update all the zombie
	for (auto z : m_zombies)
	{
		z->update(m_levels[m_currentLevel]->getLevelData(),
			m_humans, m_zombies, deltaTime);
	}

	for (int i = 0; i < m_zombies.size(); ++i)
	{
		// Collide with other zombies and push them back
		for (int j = i + 1; j < m_zombies.size(); ++j)
		{
			m_zombies[i]->collideWithAgent(m_zombies[j]);
		}

		// Collide with humans, and turn them into zombies
		// START AT 1, 0 IS THE PLAYER
		for (int j = 1; j < m_humans.size(); ++j)
		{
			if (m_zombies[i]->collideWithAgent(m_humans[j]))
			{
				// Add the new zombie
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
				// Delete the human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
			}
		}

		// Collide with player
		if (m_zombies[i]->collideWithAgent(m_player))
		{
			Bengine::fatalError("YOU LOSE");
		}
	}

	// Update collisions
	for (int i = 0; i < m_humans.size(); ++i)
	{
		for (int j = i + 1; j < m_humans.size(); ++j)
		{
			m_humans[i]->collideWithAgent(m_humans[j]);
		}
	}
}

// TODO: Collide Bullets with humans and win condition + stats
void MainGame::updateBullets(float deltaTime)
{
	// Update and collide with world
	for (int i = 0; i < m_bullets.size(); )
	{
		// Does bullet collide with a wall?
		if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime))
		{
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else
		{
			++i;
		}
	}

	// Collide with humans
	for (int i = 0; i < m_bullets.size(); ++i)
	{
		for (int j = 0; j < m_zombies.size();)
		{
			// Check collosion
			if (m_bullets[i].collideWithAgent(m_zombies[j]))
			{
				// Blood at the position where zombie got shot
				addBlood(m_bullets[i].getPosition(), 5);

				// Damage zombie and if dead delete
				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage()))
				{
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
				}
				else
				{
					++j;
				}

				// Remove bullet
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				i--;
				// Since bullet died
				break;
			}
			else
			{
				++j;
			}
		}
		++i;
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			m_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram.use();

	glActiveTexture(GL_TEXTURE0);

	// Make sure the texture uses texture 0
	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	m_levels[m_currentLevel]->draw();

	// Start drawing on agent batch
	m_agentSpriteBatch.begin();

	// Draw the humans
	glm::vec2 agentDims(AGENT_RADIUS * 2.0f);
	for (auto h : m_humans)
	{
		if (m_camera.isBoxInView(h->getPosition(), agentDims))
		{
			h->draw(m_agentSpriteBatch);
		}
	}
	// Draw the zombies
	for (auto z : m_zombies)
	{
		if (m_camera.isBoxInView(z->getPosition(), agentDims))
		{
			z->draw(m_agentSpriteBatch);
		}
	}
	// Draw the bullets
	for (auto b : m_bullets)
	{
		b.draw(m_agentSpriteBatch);
	}

	// End drawing on agent batch
	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	// Render the particles
	m_particleEngine.draw(&m_agentSpriteBatch);

	// Render the HUD
	drawHUD();

	m_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	m_window.swapBuffer();
}

void MainGame::drawHUD()
{
	// TOOD: Make this save
	char buffer[256];

	// Use the hud camera for drawing the hud
	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint pUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	// Start rendering the HUD

	sprintf_s(buffer, "NUMBER OF ZOMBIES REMAINING %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, 
		glm::vec2(0, 0), 
		glm::vec2(.5f), 0.0f,
		Bengine::ColorRGBA8(255, 255, 255, 255));



	// End rendering the HUD

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2 & position, int numParticles)
{
	static std::mt19937 re(time(nullptr));
	static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

	glm::vec2 vel(1.0f, 0.0f);
	Bengine::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; ++i)
	{
		m_bloodParticleBatch->addParticle(position, glm::rotate(vel, randAngle(re)), color, 20.0f);
	}
}
