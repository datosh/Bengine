#pragma once

#include "entity.h"

#include <string>

class Brick : public Entity 
{
public:
	Brick();
	virtual ~Brick();


	virtual void init(MainGame * mainGame) override;
	virtual void update(MainGame * mainGame, float deltaTime) override;
	virtual void draw(Bengine::SpriteBatch & spriteBatch) override;

	void init(glm::vec2 position, glm::vec2 size, Bengine::ColorRGBA8 color, unsigned int points=15u);

	// GETTER SETTER
	unsigned int get_points() const { return m_points; }

	// Sets the current brick to dead so it can be removed
	void kill();
	bool is_dead() const { return !m_alive; }
	bool is_alive() const { return m_alive; }

private:

	Bengine::ColorRGBA8 m_color;
	std::string m_texturePath;
	bool m_alive;
	unsigned int m_points;
};

struct DeleteIfBrickDead
{
	void operator()(Brick* &ptr) const
	{
		auto brk = *ptr;
		bool brk_dead = brk.is_dead();
		if (brk_dead) {
			Brick* tmp = ptr;
			ptr = nullptr;
			delete tmp;
		}
	}
};