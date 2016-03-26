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

private:

	Bengine::ColorRGBA8 m_color;
	std::string m_texturePath;
	unsigned int m_points;
};