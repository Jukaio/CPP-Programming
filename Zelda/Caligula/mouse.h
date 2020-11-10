// mouse.h

#ifndef INCLUDED_MOUSE_H
#define INCLUDED_MOUSE_H

#include <SDL_mouse.h>

// Hold input data about the current and last frame

struct mouse
{
	mouse();
	~mouse();
	void update();
	bool button_down(unsigned int p_button);
	bool button_pressed(unsigned int p_button);
	bool button_released(unsigned int p_button);
	void update_button(unsigned int p_button, bool p_value);
	void update_wheel(int p_x, int p_y);
	void update_motion(int p_x, int p_y);
	int  get_motion_x();
	int  get_motion_y();
	int  get_position_x();
	int  get_position_y();
	int  get_wheel_motion_x();
	int  get_wheel_motion_y();
	int  get_wheel_position_x();
	int  get_wheel_position_y();

private:
	static const unsigned int m_buttons = 5;
	bool m_current_frame[m_buttons];
	bool m_last_frame[m_buttons];
	int m_current_wheel_x;
	int m_last_wheel_x;
	int m_current_wheel_y;
	int m_last_wheel_y;
	int m_current_x;
	int m_last_x;
	int m_current_y;
	int m_last_y;
};

#endif // !INCLUDED_MOUSE_H