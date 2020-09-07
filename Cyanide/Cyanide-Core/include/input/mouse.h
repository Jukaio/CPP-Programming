// mouse.h

#ifndef INCLUDED_MOUSE_H
#define INCLUDED_MOUSE_H

#include "utilities/cyanide_math.h"
#include <vector>

struct mouse
{
	enum class button
	{
		LEFT = 1,
		MIDDLE,
		RIGHT,
		BUTTON_04,
		BUTTON_05,
		NUM_KEYS
	};

	mouse();
	~mouse();
	void update();
	bool button_down(short p_button);
	bool button_pressed(short p_button);
	bool button_released(short p_button);
	void update_button(short p_button, bool p_value);
	void update_wheel(int p_x, int p_y);
	void update_motion(int p_x, int p_y);

	vector2 get_motion();
	vector2 get_position();
	vector2 get_wheel_motion();
	vector2 get_wheel_position();

private:
	std::vector<bool> m_current_frame;
	std::vector<bool> m_previous_frame;

	vector2 m_current_wheel = vector2::zero;
	vector2 m_last_wheel = vector2::zero;
	vector2 m_current_pos = vector2::zero;
	vector2 m_last_pos = vector2::zero;
}; 

#endif // !INCLUDED_MOUSE_H
