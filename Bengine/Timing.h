#pragma once

#include "stdafx.h"
#include <SDL.h>

namespace Bengine {
	class FPSLimiter {
	public:
		FPSLimiter();

		void init(float maxFPS);
		void setMaxFPS(float maxFPS);
		void begin();
		// returns the current FPS
		float end();

	private:
		void calculateFPS();
		float m_maxFPS, m_fps, m_frameTime;
		unsigned int m_startTicks;
	};
}
