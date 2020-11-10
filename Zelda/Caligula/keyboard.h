// keyboard.h

#ifndef INCLUDED_KEYBOARD_H
#define INCLUDED_KEYBOARD_H

#include <SDL_scancode.h>

// Hold input data about the current and last frame

struct keyboard
{
	keyboard();
	~keyboard();
	void update();
	bool key_down(SDL_Scancode p_key);
	bool key_pressed(SDL_Scancode p_key);
	bool key_released(SDL_Scancode p_key);
	void update_key(SDL_Scancode p_key, bool p_value);

private:
	bool m_current_frame[SDL_NUM_SCANCODES];
	bool m_last_frame[SDL_NUM_SCANCODES];
};

#endif // !INCLUDED_KEYBOARD_H