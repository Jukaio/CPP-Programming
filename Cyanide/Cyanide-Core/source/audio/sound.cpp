// sound.cpp

#include <assert.h>
#include <iostream>

#include <SDL_mixer.h>
#include "audio/sound.h"

sound::sound(Mix_Chunk* p_chunk)
	: m_chunk(p_chunk)
	, m_channel(-1)
	, m_volume(SDL_MIX_MAXVOLUME)
{
	
}

sound::~sound()
{
	if(!m_chunk)
		Mix_FreeChunk(m_chunk);
	m_chunk = nullptr;
}

void sound::play(int p_loops, int p_timer)
{
	if(!(m_channel < 0))
		if(Mix_Playing(m_channel))
			m_channel = -1;

	m_channel = Mix_PlayChannelTimed(m_channel, m_chunk, p_loops, p_timer);
	if(m_channel == -1)
	{
		assert(!"Error in sound.cpp play\n");
		std::cout << "Error in channel " <<  m_channel << "\n" << Mix_GetError() << "!\n";
	}
}

void sound::pause()
{
	if(m_channel < 0)
		return;

	Mix_Pause(m_channel);
}

void sound::resume()
{
	if(m_channel < 0)
		return;

	Mix_Resume(m_channel);
}

void sound::stop()
{
	if(m_channel < 0)
		return;

	Mix_HaltChannel(m_channel);
}

void sound::set_volume(int p_volume)
{
	m_volume = Mix_Volume(m_channel, p_volume);
	if(m_volume == -1)
	{
		assert(!"Error in sound.cpp set_volume\n");
		std::cout << "Error in channel " << m_channel << "\n" << Mix_GetError() << "!\n";
	}
}

void sound::fade_in(int p_speed, int p_loops, int p_timer)
{
	m_channel = Mix_FadeInChannelTimed(m_channel, m_chunk, p_loops, p_speed, p_timer);
	if(m_channel == -1)
	{
		assert(!"Error in sound.cpp play\n");
		std::cout << "Error in channel " << m_channel << "\n" << Mix_GetError() << "!\n";
	}
}

void sound::fade_out(int p_speed)
{
	if(m_channel < 0)
		return;

	Mix_FadeOutChannel(m_channel, p_speed);
}

bool sound::is_playing()
{
	if(m_channel < 0)
		return false;

	return Mix_Playing(m_channel);
}

bool sound::is_paused()
{
	if(m_channel < 0)
		return false;

	return Mix_Paused(m_channel);
}

bool sound::is_fading()
{
	if(m_channel < 0)
		return false;

	return Mix_FadingChannel(m_channel);
}

int sound::control_channel(int p_channel)
{
	if(!(p_channel < 0))
		if(Mix_Playing(p_channel))
			p_channel = -1;
	return p_channel;
}

