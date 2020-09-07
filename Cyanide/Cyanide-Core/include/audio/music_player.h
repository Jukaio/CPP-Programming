// music_player.h

#ifndef INCLUDED_MUSIC_PLAYER_H
#define INCLUDED_MUSIC_PLAYER_H

#include <string>

#include "utilities/resource_cache.h"

// Macros declared in sound.h and here
#define LOOP_FOREVER -1
#define MAX_VOLUME 128

struct _Mix_Music;

struct music_player
{
	music_player();
	~music_player();

	void load(std::string p_title, std::string p_path);

	void play(std::string p_title, int p_loops = LOOP_FOREVER);
	void pause();
	void resume();
	void stop();

	void set_volume(int p_volume);
	int get_volume();
	void fade_in(std::string p_title, int p_speed, int p_loops = LOOP_FOREVER);
	void fade_out(int p_speed);

	bool is_playing();
	bool is_paused();
	bool is_fading();

private:
	resource_cache<std::string, _Mix_Music> m_cache; 
	std::string m_current_title;

};

#endif // !INCLUDED_MUSIC_PLAYER_H


