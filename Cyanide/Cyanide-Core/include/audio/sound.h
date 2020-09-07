// sound.h

#ifndef INCLUDED_SOUND_H
#define INCLUDED_SOUND_H


// Macros declared in music_player.h and here
#define LOOP_FOREVER -1
#define PLAY_FOREVER -1
#define MAX_VOLUME 128

struct Mix_Chunk;

struct sound
{
	sound(Mix_Chunk* p_chunk);
	~sound();

	void play(int p_loops = LOOP_FOREVER, int p_timer = PLAY_FOREVER);
	void pause();
	void resume();
	void stop();

	void set_volume(int p_volume);

	void fade_in(int p_speed, int p_loops = LOOP_FOREVER, int p_timer = PLAY_FOREVER);
	void fade_out(int p_speed);

	bool is_playing();
	bool is_paused();
	bool is_fading();

private:
	static int control_channel(int p_channel);

	Mix_Chunk* m_chunk;
	int m_volume = MAX_VOLUME;
	int m_channel = -1;
};

#endif // !INCLUDED_SOUND_H



