// sound.h

#ifndef INCLUDED_SOUND_H
#define INCLUDED_SOUND_H

// Wrapper for Mix_Chunk* to ensure correct destruction (c++ delete keyword calls the c-style destory function)
// Additionally some functionality sits in this object 

struct Mix_Chunk;

struct sound
{
	sound(Mix_Chunk* p_chunk);
	~sound();

	void play(int p_loops = 0);
	void resume();
	void pause();

private:
	Mix_Chunk* m_chunk = nullptr;
	int m_channel = -1;
};

#endif // !INCLUDED_SOUND_H



