// music.cpp

#include "music.h"
#include <SDL_mixer.h>

music::music(_Mix_Music* p_music)
	: m_music(p_music)
{
}

music::~music()
{
	if(m_music != nullptr)
		Mix_FreeMusic(m_music);
	m_music = nullptr;
}

_Mix_Music* music::get()
{
	return m_music;
}
