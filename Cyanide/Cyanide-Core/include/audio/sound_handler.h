// sound_handler.h

#ifndef INCLUDED_SOUND_HANDLER_H
#define INCLUDED_SOUND_HANDLER_H

#include "utilities/resource_cache.h"
#include "sound.h"

struct sound_handler
{
	struct sound_info
	{
		int m_id;
		sound* m_sound;
	};

	sound_handler();
	~sound_handler();

	static void clean_channel(int channel);

	sound_info create_sound(std::string path);

private:
	resource_cache<int, sound> m_cache;
	int m_size;
};

#endif // !INCLUDED_SOUND_HANDLER_H
