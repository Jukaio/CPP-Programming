// music.h

#ifndef INCLUDED_MUSIC_H
#define INCLUDED_MUSIC_H

// wrapper struct to hold a Mix_Music*
// It's most important job is to get called by the resource cache
// so it gets destroyed without memory leak
// The Mix_Music object cannot get destroyed by delete, rather by a c-style destroy/free function
// the destructor of this struct is a wrapper method for Mix_FreeMusic

struct _Mix_Music;

struct music
{
	music(_Mix_Music* p_music);
	~music();

	_Mix_Music* get();

private:
	_Mix_Music* m_music = nullptr;
};

#endif // !INCLUDED_MUSIC_H



