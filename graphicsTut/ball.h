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
	bool is_alive() const { return m_alive; }
	bool is_dead() const { return !m_alive; }

	// SETTER
	void set_velocity(glm::vec2& velocity) { m_velocity = velocity; }
	void set_position(glm::vec2& position) { m_position = position; }

private:
	glm::vec2 m_velocity;
	bool m_alive;

};

struct DeleteIfBallDead
{
	void operator()(Ball* &ptr) const
	{
		auto brk = *ptr;
		bool brk_dead = brk.is_dead();
		if (brk_dead) {
			Ball* tmp = ptr;
			ptr = nullptr;
			delete tmp;
		}
	}
};
