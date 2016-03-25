#pragma once

#include "stdafx.h"
#include "Errors.h"
#include <SDL_mixer.h>

namespace Bengine {

	class SoundEffect {
	public:
		friend class AudioEngine;

		void play(int loops = 0);

	private:
		Mix_Chunk* m_chunk = nullptr;

	};

	class Music {
	public:
		friend class AudioEngine;

		/// Plays the audio file
		/// @param loops: loops == -1 => loop forever, otherwise loops times
		void play(int loops = -1);

		static void pause();
		static void stop();
		static void resume();

	private:
		Mix_Music* m_music = nullptr;
	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void init();
		void destroy();

		SoundEffect loadSoundEffect(const std::string& filePath);
		Music loadMusic(const std::string& filePath);

	private:
		std::map<std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musicMap;

		bool m_isInitialized = false;
	};
}



