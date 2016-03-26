#pragma once

#include "entity.h"

class BrickParticle : public Entity
{
public:
	BrickParticle();
	~BrickParticle();

	virtual void init(MainGame * mainGame) override;
	virtual void update(MainGame * mainGame, float deltaTime) override;
	virtual void draw(Bengine::SpriteBatch & spriteBatch) override;

private:
	glm::vec2 m_velocity;

	float m_shrink_factor;

};