// input_handler.cpp

#include "input/input_handler.h"
#include <SDL.h>
#include <iostream>
#include <assert.h>
#include <string>

void input_handler::set_window(SDL_Window* p_window)
{
	if(throw_error("Parameter in set_window == nullptr", p_window))
		return;
	m_window.set_window(p_window);
}

bool input_handler::window_resized()
{
	if(throw_error("m_window->SDL_Window* not set - window_resized", m_window.is_set()))
		return false;
	return m_window.resized();
}
vector2 input_handler::get_window_size()
{
	if(throw_error("m_window->SDL_Window* not set - get_window_size", m_window.is_set()))
		return {-1, -1 };
	return { m_window.window_w(), m_window.window_h() };
}

bool input_handler::is_down(keyboard::key p_key)
{
	return m_keyboard.key_down((short)p_key);
}
bool input_handler::is_down(mouse::button p_button)
{
	return m_mouse.button_down((short)p_button);
}

bool input_handler::pressed(keyboard::key p_key)
{
	return m_keyboard.key_pressed((short)p_key);
}
bool input_handler::pressed(mouse::button p_button)
{
	return m_mouse.button_pressed((short)p_button);
}

bool input_handler::released(keyboard::key p_key)
{
	return m_keyboard.key_released((short)p_key);
}
bool input_handler::released(mouse::button p_button)
{
	return m_mouse.button_released((short)p_button);
}

vector2 input_handler::mouse_motion()
{
    return m_mouse.get_motion();
}

vector2 input_handler::mouse_position()
{
    return m_mouse.get_position();
}

vector2 input_handler::mouse_wheel_motion()
{
    return m_mouse.get_wheel_motion();
}

vector2 input_handler::mouse_wheel_position()
{
    return m_mouse.get_wheel_position();
}

bool input_handler::handle_events()
{
	m_keyboard.update();
	m_mouse.update();
	m_window.reset();

	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_EventType::SDL_KEYDOWN:			m_keyboard.update_key(event.key.keysym.scancode, true);		break;
			case SDL_EventType::SDL_KEYUP:				m_keyboard.update_key(event.key.keysym.scancode, false);	break;
			case SDL_EventType::SDL_MOUSEBUTTONDOWN:	m_mouse.update_button(event.button.button, true);			break;
			case SDL_EventType::SDL_MOUSEBUTTONUP:		m_mouse.update_button(event.button.button, false);			break;
			case SDL_EventType::SDL_MOUSEWHEEL:			m_mouse.update_wheel(event.wheel.x, event.wheel.y);			break;
			case SDL_EventType::SDL_MOUSEMOTION:		m_mouse.update_motion(event.motion.x, event.motion.y);		break;
			case SDL_EventType::SDL_WINDOWEVENT:		
														switch(event.window.event)
														{
															case SDL_WINDOWEVENT_RESIZED:
																m_window.update_resize(true);
															break;
														}
														break;
			case SDL_EventType::SDL_QUIT:																			return false;
		}
	}
	return true;
}

bool input_handler::throw_error(const char* error_message, bool condition)
{
	if(!condition)
	{
		std::cout << error_message << "\n";
		assert(!error_message);
		return true;
	}
	return false;
}
