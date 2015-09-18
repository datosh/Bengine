#pragma once

#include "stdafx.h"
#include "Zombie.h"

#include <Bengine\SpriteBatch.h>

template<typename T>
class EntityList
{
public:
	EntityList();
	~EntityList();

	void deleteDead();
	void drawAll(Bengine::SpriteBatch & spriteBatch);
	void updateAll();

	T* getClosest(Zombie entity);

	std::vector<T> entities;
	auto begin();
	auto end();
};

template<typename T>
inline EntityList<T>::EntityList()
{
}

template<typename T>
inline EntityList<T>::~EntityList()
{
}

template<typename T>
inline void EntityList<T>::deleteDead()
{
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](T t) {return !t.alive();}), entities.end());
}

template<typename T>
inline void EntityList<T>::drawAll(Bengine::SpriteBatch & spriteBatch)
{
	for (T e : entities)
	{
		e.draw(spriteBatch);
	}
}

template<typename T>
inline void EntityList<T>::updateAll()
{
	for (T e : entities)
	{
		e.update();
	}
}

template<typename T>
inline T* EntityList<T>::getClosest(Zombie entity)
{
	T* clostest;
	//TODO: Change to max float
	float distance = 999999.0f;

	for (T e : entities)
	{
		glm::vec2 distVec = e.getPos() - entity.getPos();
		if (distance > glm::length(distVec))
		{
			clostest = &e;
			distance = glm::length(distVec);
		}
	}

	return clostest;
}

template<typename T>
inline auto EntityList<T>::begin()
{
	return entities.begin();
}

template<typename T>
inline auto EntityList<T>::end()
{
	return entities.end();
}
