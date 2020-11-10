// keyboard.cpp

#include "keyboard.h"

keyboard::keyboard()
{
	for (int i = 0; i < SDL_Scancode::SDL_NUM_SCANCODES; i++)
	{
		m_last_frame[i] = 0;
		m_current_frame[i] = 0;
	}
}

keyboard::~keyboard()
{
}

void keyboard::update()
{
	for (int i = 0; i < SDL_Scancode::SDL_NUM_SCANCODES; i++)
	{
		m_last_frame[i] = m_current_frame[i];
	}
}

bool keyboard::key_down(SDL_Scancode p_key)
{
	return m_current_frame[p_key];
}

bool keyboard::key_pressed(SDL_Scancode p_key)
{
	return (m_last_frame[p_key] == false && m_current_frame[p_key] == true);
}

bool keyboard::key_released(SDL_Scancode p_key)
{
	return (m_last_frame[p_key] == true && m_current_frame[p_key] == false);
}

void keyboard::update_key(SDL_Scancode p_key, bool p_value)
{
	m_current_frame[p_key] = p_value;
}

