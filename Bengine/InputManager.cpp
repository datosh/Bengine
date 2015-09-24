#include "InputManager.h"


namespace Bengine {
	InputManager::InputManager() : m_mouseCoords(0)
	{
	}


	InputManager::~InputManager()
	{
	}

	void InputManager::update()
	{
		for (auto& it : m_keyMap)
		{
			m_prevKeyMap[it.first] = it.second;
		}
	}

	void InputManager::pressKey(unsigned int keyID)
	{
		m_keyMap[keyID] = true;
	}

	void InputManager::releaseKey(unsigned int keyID)
	{
		m_keyMap[keyID] = false;
	}

	void InputManager::setMouseCoords(float x, float y)
	{
		m_mouseCoords.x = x;
		m_mouseCoords.y = y;
	}

	bool InputManager::isKeyDown(unsigned int keyID)
	{
		auto it = m_keyMap.find(keyID);
		if (it != m_keyMap.end())
		{
			return it->second;
		}
		return false;
	}

	bool InputManager::isKeyPressed(unsigned int keyID)
	{
		return isKeyDown(keyID) && !wasKeyDown(keyID);
	}

	bool InputManager::wasKeyDown(unsigned int keyID)
	{
		auto it = m_prevKeyMap.find(keyID);
		if (it != m_prevKeyMap.end())
		{
			return it->second;
		}
		return false;
	}
}