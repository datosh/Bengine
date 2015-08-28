#include "Timing.h"

namespace Bengine {
	FPSLimiter::FPSLimiter()
	{
	}
	void FPSLimiter::init(float maxFPS)
	{
		setMaxFPS(maxFPS);
	}
	void FPSLimiter::setMaxFPS(float maxFPS)
	{
		m_maxFPS = maxFPS;
	}
	void FPSLimiter::begin()
	{
		m_startTicks = SDL_GetTicks();
	}
	float FPSLimiter::end()
	{
		calculateFPS();

		float frameTicks = static_cast<float>(SDL_GetTicks() - m_startTicks);
		if (1000.0f / m_maxFPS > frameTicks)
		{
			SDL_Delay(static_cast<Uint32>(1000.0f / m_maxFPS - frameTicks));
		}
		return m_fps;
	}
	void FPSLimiter::calculateFPS()
	{
		// Init once
		static int currentFrame = 0;
		static const int NUM_SAMPLES = 100;
		static float frameTimes[NUM_SAMPLES];
		static float prevTicks = static_cast<float>(SDL_GetTicks());

		// Get delta
		float currentTicks = static_cast<float>(SDL_GetTicks());
		m_frameTime = currentTicks - prevTicks;
		frameTimes[currentFrame++ % NUM_SAMPLES] = m_frameTime;
		prevTicks = currentTicks;

		int count;
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}

		float frameTimeAverage = 0;
		for (auto time : frameTimes)
		{
			frameTimeAverage += time;
		}
		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
		{
			m_fps = 1000.0f / frameTimeAverage;
		}
		else
		{
			m_fps = 60.0f;
		}
	}
}