#pragma once

#include <glm.hpp>
#include <Bengine\SpriteBatch.h>
#include <Bengine\ResourceManager.h>

class MainGame;
class Level;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	// VIRTUAL ABSTRACT FUNCTIONS
	/** Define the entity specific behaviour, always need to be implemented by child */
	virtual void init(MainGame* mainGame) = 0;
	virtual void update(MainGame* mainGame, float deltaTime) = 0;
	virtual void draw(Bengine::SpriteBatch& spriteBatch) = 0;

	// VIRTUAL FUNCTIONS
	/** Returns false if the current position doesnt collide with non-entity */
	virtual bool collideWithLevel(const Level& level);
	/** Returns nullptr if the current position doesnt collide with any other entity */
	virtual Entity* collideWithEntities(const std::vector<Entity*>& entitys);
	/** Returns false if the current position doesnt collide with the entity */
	virtual bool collideWithEntity(const Entity* entity);

	// Functions to check the relativ position to another entity
	virtual bool below(const Entity* entity, float delta=10);
	virtual bool above(const Entity* entity, float delta=10);
	virtual bool left_of(const Entity* entity, float delta=10);
	virtual bool right_of(const Entity* entity, float delta=10);

	/** Returns the current position */
	glm::vec2 get_position() const { return m_position; }
	glm::vec2 get_size() const { return glm::vec2(m_width, m_height); }
	float get_width() const { return m_width; }
	float get_height() const { return m_height; }

	float right() const { return m_position.x + m_width; }
	float left() const { return m_position.x; }
	float top() const { return m_position.y + m_height; }
	float bottom() const { return m_position.y; }

protected:
	glm::vec2 m_position;
	float m_speed;
	float m_width, m_height;
};