#pragma once

#include "stdafx.h"

#include <glm.hpp>
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>

class Entity
{
public:
	Entity();
	Entity(glm::vec2 pos, glm::vec2 dir, float speed, float size, Bengine::Color color);
	~Entity();

	// Virtual
	virtual bool moveRel(glm::vec2 delta) = 0;
	virtual bool moveAbs(glm::vec2 pos) = 0;
	virtual void draw(Bengine::SpriteBatch& spriteBatch) = 0;
	virtual bool update() = 0;

	// Methods
	bool overlaps(Entity& entity);

	// Setter
	void setSpeed(float speed) { m_speed = speed; }
	void setSize(float size) { m_size = size; }
	void setPos(glm::vec2 pos) { m_pos = pos; }
	void setDir(glm::vec2 dir) { m_dir = dir; }
	void setColor(Bengine::Color color) { m_color = color; }
	void die();

	// Getter
	float getSpeed() { return m_speed; }
	float getSize() { return m_size; }
	glm::vec2 getPos() { return m_pos; }
	glm::vec2 getDir() { return m_dir; }
	Bengine::Color getColor() { return m_color; }
	bool alive();

protected:
	float m_speed, m_size;
	glm::vec2 m_pos, m_dir;
	Bengine::Color m_color;

	bool m_alive;
};

