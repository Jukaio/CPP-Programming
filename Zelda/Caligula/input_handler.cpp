// input_handler.cpp

#include "input_handler.h"
#include <SDL.h>

bool input_handler::key_down(SDL_Scancode p_key)
{
	return m_keyboard.key_down(p_key);
}

bool input_handler::key_pressed(SDL_Scancode p_key)
{
	return m_keyboard.key_pressed(p_key);
}

bool input_handler::key_released(SDL_Scancode p_key)
{
	return m_keyboard.key_released(p_key);
}

bool input_handler::mouse_button_down(unsigned int p_button)
{
	return false;
}

bool input_handler::mouse_button_pressed(unsigned int p_button)
{
	return false;
}

bool input_handler::mouse_button_released(unsigned int p_button)
{
	return false;
}

int input_handler::mouse_motion_x()
{
	return m_mouse.get_motion_x();
}

int input_handler::mouse_motion_y()
{
	return m_mouse.get_motion_y();
}

int input_handler::mouse_position_x()
{
	return m_mouse.get_position_x();
}

int input_handler::mouse_position_y()
{
	return m_mouse.get_position_y();
}

int input_handler::mouse_wheel_motion_x()
{
	return m_mouse.get_wheel_motion_x();
}

int input_handler::mouse_wheel_motion_y()
{
	return m_mouse.get_wheel_motion_y();
}

int input_handler::mouse_wheel_position_x()
{
	return m_mouse.get_wheel_position_x();
}

int input_handler::mouse_wheel_position_y()
{
	return m_mouse.get_wheel_position_y();
}

void input_handler::handle_events()
{
	m_keyboard.update();
	m_mouse.update();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_EventType::SDL_KEYDOWN: m_keyboard.update_key(event.key.keysym.scancode, true); break;
			case SDL_EventType::SDL_KEYUP: m_keyboard.update_key(event.key.keysym.scancode, false); break;
			case SDL_EventType::SDL_MOUSEBUTTONDOWN: m_mouse.update_button(event.button.button, true); break;
			case SDL_EventType::SDL_MOUSEBUTTONUP: m_mouse.update_button(event.button.button, false); break;
			case SDL_EventType::SDL_MOUSEWHEEL: m_mouse.update_wheel(event.wheel.x, event.wheel.y); break;
			case SDL_EventType::SDL_MOUSEMOTION: m_mouse.update_motion(event.motion.x, event.motion.y); break;
		}
	}
}
