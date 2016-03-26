#pragma once

#include <Bengine\InputManager.h>

#include "entity.h"

class Paddle : public Entity
{
public:
	Paddle();
	virtual ~Paddle();

	virtual void init(MainGame* mainGame) override;
	virtual void update(MainGame* mainGame, float deltaTime) override;
	virtual void draw(Bengine::SpriteBatch & spriteBatch) override;

	// SETTER
	void setInputManager(Bengine::InputManager* inputManager) { m_inputManager = inputManager; }

	// ATTRIBUTES
	Bengine::InputManager* m_inputManager;

private:
	float m_to_grow;
	float m_max_width;
};

