// input_handler.h

#ifndef INCLUDED_INPUT_HANDLER
#define INCLUDED_INPUT_HANDLER

#include "keyboard.h"
#include "mouse.h"
#include "window.h"

struct input_handler
{
	void set_window(SDL_Window* p_window);
	bool window_resized();
	vector2 get_window_size();

	bool is_down(keyboard::key p_key);
	bool is_down(mouse::button p_button);

	bool pressed(keyboard::key p_key);
	bool pressed(mouse::button p_button);

	bool released(keyboard::key p_key);
	bool released(mouse::button p_button);

	vector2 mouse_motion();
	vector2 mouse_position();
	vector2 mouse_wheel_motion();
	vector2 mouse_wheel_position();
	bool handle_events();

private:
	bool throw_error(const char* error_message, bool condition);

	keyboard m_keyboard;
	mouse m_mouse;
	window_info m_window;
};

#endif // !INCLUDED_INPUT_HANDLER

