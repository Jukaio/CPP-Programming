// sound.cpp

#include "sound.h"
#include <SDL_mixer.h>

sound::sound(Mix_Chunk* p_chunk)
	: m_chunk(p_chunk)
{
}

sound::~sound()
{
	if(m_chunk != nullptr)
		Mix_FreeChunk(m_chunk);
	m_chunk = nullptr;
}

void sound::play(int p_loops)
{
	m_channel = Mix_PlayChannel(-1, m_chunk, p_loops);
}

void sound::resume()
{
	Mix_Resume(m_channel);
}

void sound::pause()
{
	Mix_Pause(m_channel);
}
