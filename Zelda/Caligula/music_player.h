// music_player.h

#ifndef INCLUDED_MUSIC_PLAYER_H
#define INCLUDED_MUSIC_PLAYER_H

#include "resource_cache.h"
#include "music.h"

// It works like a mp3 player with out a playlist/playing queue
// Keeps music in a cache and has wrapper methods for the SDL Music functions
// in the music/music player case, a lot of functionality is in the music manager class,
// rather than in the resource class

struct music_player
{
	bool load(std::string p_id, std::string p_path);
	bool unload(std::string p_id);

	bool fade_in(std::string p_id, int p_ms = 2000, int p_loops = -1);
	void fade_out(int p_ms);

	bool play(std::string p_id, int p_loops = -1);
	void pause();
	void resume();
	void stop();

	void set_volume(int p_volume);

private:
	resource_cache<music> m_cache;
};

#endif // !INCLUDED_MUSIC_PLAYER_H



