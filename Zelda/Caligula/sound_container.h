// sound_container.h

#ifndef INCLUDED_SOUND_CONTAINER_H
#define INCLUDED_SOUND_CONTAINER_H

#include "resource_cache.h"
#include "sound.h"

// Unlike music/music_player, sound/sound_container has most of the functionalities
// in the resource object rather than the manager one
// the sound container is just supposed to keep the data in a cache, rather than giving any sort of functionality
// what allows entities to make use of sounds directly
// option B would have been static functions

struct sound_container
{
	bool load(std::string p_id, std::string p_path);
	bool unload(std::string p_id);
	sound* get(std::string p_id);

private:
	resource_cache<sound> m_cache;
};

#endif // !INCLUDED_SOUND_MANAGER_H



