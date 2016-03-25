#include "AudioEngine.h"

namespace Bengine {

	AudioEngine::AudioEngine()
	{
		// Empty
	}


	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init()
	{
		if (m_isInitialized)
		{
			fatalError("Tried to init Audio Engine twice!\n");
		}

		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			fatalError("Mix_Init error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		{
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}

		m_isInitialized = true;
	}

	void AudioEngine::destroy()
	{
		if (m_isInitialized)
		{
			for (auto& it : m_effectMap)
			{
				Mix_FreeChunk(it.second);
			}

			for (auto& it : m_musicMap)
			{
				Mix_FreeMusic(it.second);
			}

			m_effectMap.clear();
			m_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();

			m_isInitialized = false;
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string & filePath)
	{
		auto it = m_effectMap.find(filePath);
		
		SoundEffect effect;

		if (it == m_effectMap.end())
		{
			// Failed to find must load
			Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());
			if (chunk == nullptr)
			{
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}
			effect.m_chunk = chunk;

			// And save in map
			m_effectMap[filePath] = chunk;
		}
		else
		{
			// Found, so load from cache
			effect.m_chunk = it->second;
		}

		return effect;
	}

	Music AudioEngine::loadMusic(const std::string & filePath)
	{
		auto it = m_musicMap.find(filePath);

		Music music;

		if (it == m_musicMap.end())
		{
			// Failed to find must load
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());
			if (mixMusic == nullptr)
			{
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}
			music.m_music = mixMusic;

			// And save in map
			m_musicMap[filePath] = mixMusic;
		}
		else
		{
			// Found, so load from cache
			music.m_music = it->second;
		}

		return music;
	}

	void SoundEffect::play(int loops)
	{
		if (Mix_PlayChannel(-1, m_chunk, loops) == -1)
		{
			static int channel_index = 0;
			if (Mix_PlayChannel(channel_index++, m_chunk, loops) == -1)
			{
				fatalError("Mix_PlayChannel error: " + std::string(Mix_GetError()));
			}
		}
	}

	void Music::play(int loops)
	{
		Mix_PlayMusic(m_music, loops);
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

}
