#pragma once

#include "entity.h"

class Ball : public Entity
{
public:
	Ball();
	virtual ~Ball();

	virtual void init(MainGame * mainGame) override;
	virtual void update(MainGame * mainGame, float deltaTime) override;
	virtual void draw(Bengine::SpriteBatch & spriteBatch) override;
	
	// GETTER
	glm::vec2 get_velocity() const { return m_velocity; }

	// SETTER
	void set_velocity(glm::vec2& velocity) { m_velocity = velocity; }

private:
	glm::vec2 m_velocity;

};
