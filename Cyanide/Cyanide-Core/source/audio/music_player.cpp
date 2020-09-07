// music_player.cpp

#include <SDL_mixer.h>

#include "audio/music_player.h"

music_player::music_player()
	: m_current_title("")
{

}

music_player::~music_player()
{
}

void music_player::load(std::string p_title, std::string p_path)
{
	Mix_Music* music = Mix_LoadMUS(p_path.c_str());
	if(music)
		m_cache.add(p_title, music);
}

void music_player::play(std::string p_title, int p_loops)
{
	if(m_cache.exists(p_title))
		if(!Mix_PlayMusic(m_cache.get(p_title), p_loops))
			/* TODO: error message */;
}

void music_player::pause()
{
	if(Mix_PlayingMusic())
		Mix_PauseMusic();
}

void music_player::resume()
{
	if(Mix_PausedMusic())
		Mix_ResumeMusic();
}

void music_player::stop()
{
	Mix_HaltMusic();
}

void music_player::set_volume(int p_volume)
{
	Mix_VolumeMusic(p_volume);
}

int music_player::get_volume()
{
	return Mix_VolumeMusic(-1);
}

void music_player::fade_in(std::string p_title, int p_speed, int p_loops)
{
	if(m_cache.exists(p_title))
		if(!Mix_FadeInMusic(m_cache.get(p_title), p_loops, p_speed))
			/* TODO: error message */;
}

void music_player::fade_out(int p_speed)
{
	Mix_FadeOutMusic(p_speed);
}

bool music_player::is_playing()
{
	return Mix_PlayingMusic();
}

bool music_player::is_paused()
{
	return Mix_PausedMusic();
}

bool music_player::is_fading()
{
	return Mix_FadingMusic();
}
