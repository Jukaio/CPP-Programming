// input_handler.h

#ifndef INCLUDED_INPUT_HANDLER_H
#define INCLUDED_INPUT_HANDLER_H

#include "mouse.h"
#include "keyboard.h"

// Uses the SDL Event Handler, it's nothing impressive
// Checks if something is happening with the mouse or keyboard in the current frame
// Based on the last frame and the current frame, a key counts as pressed, released or down,
// same applies to the mouse events but those are more coordinate based 


struct input_handler
{
	bool key_down(SDL_Scancode p_key);
	bool key_pressed(SDL_Scancode p_key);
	bool key_released(SDL_Scancode p_key);
	bool mouse_button_down(unsigned int p_button);
	bool mouse_button_pressed(unsigned int p_button);
	bool mouse_button_released (unsigned int p_button);
	int  mouse_motion_x();
	int  mouse_motion_y();
	int  mouse_position_x();
	int  mouse_position_y();
	int  mouse_wheel_motion_x();
	int  mouse_wheel_motion_y();
	int  mouse_wheel_position_x();
	int  mouse_wheel_position_y();
	void handle_events();

private:
	keyboard m_keyboard;
	mouse m_mouse;
};

#endif //!INCLUDED_INPUT_HANDLER_H