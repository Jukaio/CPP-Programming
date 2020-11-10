// sound_container.cpp

#include "sound_container.h"
#include <SDL_mixer.h>

bool sound_container::load(std::string p_id, std::string p_path)
{
	Mix_Chunk* chunk = Mix_LoadWAV(p_path.c_str());
	if(chunk == nullptr)
		return false;

	m_cache.add(p_id, new sound(chunk));
	return true;
}

bool sound_container::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

sound* sound_container::get(std::string p_id)
{
	return m_cache[p_id];
}

