#pragma once

#include "stdafx.h"

namespace Bengine {
	class InputManager
	{
	public:
		InputManager();
		~InputManager();

		void pressKey(unsigned int keyID);
		void releaseKey(unsigned int keyID);
		bool isKeyPressed(unsigned int keyID);

	private:
		std::unordered_map<unsigned int, bool> m_keyMap;

	};

}