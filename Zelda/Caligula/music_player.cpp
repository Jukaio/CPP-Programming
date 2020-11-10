// music_player.cpp

#include "music_player.h"
#include <SDL_mixer.h>

bool music_player::load(std::string p_id, std::string p_path)
{
	Mix_Music* clip = Mix_LoadMUS(p_path.c_str());
	if(clip == nullptr)
		return false;

	m_cache.add(p_id, new music(clip));
	return true;
}

bool music_player::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

bool music_player::fade_in(std::string p_id, int p_ms, int p_loops)
{
	music* clip = m_cache[p_id];
	if(clip == nullptr)
		return false;

	Mix_FadeInMusic(clip->get(), p_loops, p_ms);
	return true;
}

void music_player::fade_out(int p_ms)
{
	Mix_FadeOutMusic(p_ms);
}

bool music_player::play(std::string p_id, int p_loops)
{
	music* clip = m_cache[p_id];
	if(clip == nullptr)
		return false;

	Mix_PlayMusic(clip->get(), p_loops);
	return true;
}

void music_player::pause()
{
	Mix_PauseMusic();
}

void music_player::resume()
{
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
