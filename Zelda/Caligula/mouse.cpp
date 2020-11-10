// mouse.cpp

#include "mouse.h"

mouse::mouse()
{
	for (int i = 0; i < m_buttons; i++)
	{
		m_last_frame[i] = 0;
		m_current_frame[i] = 0;
	}
	m_current_wheel_x = 0; m_last_wheel_x = 0;
	m_current_wheel_y = 0; m_last_wheel_y = 0;
	m_current_x = 0;	m_last_x = 0;
	m_current_y = 0;	m_last_y = 0;
}

mouse::~mouse()
{
}

void mouse::update()
{
	for (int i = 0; i < m_buttons; i++)
	{
		m_last_frame[i] = m_current_frame[i];
	}
	m_last_x = m_current_x;
	m_last_y = m_current_y;
	m_last_wheel_x = m_current_wheel_x;
	m_last_wheel_y = m_current_wheel_y;
}

bool mouse::button_down(unsigned int p_button)
{
	if (p_button >= m_buttons)
		return false;

	return m_current_frame[p_button];
}

bool mouse::button_pressed(unsigned int p_button)
{
	if (p_button >= m_buttons)
		return false;

	return (m_last_frame[p_button] == false && m_current_frame[p_button] == true);
}

bool mouse::button_released(unsigned int p_button)
{
	if (p_button >= m_buttons)
		return false;

	return (m_last_frame[p_button] == true && m_current_frame[p_button] == false);
}

void mouse::update_button(unsigned int p_button, bool p_value)
{
	if (p_button >= m_buttons)
		return;

	m_current_frame[p_button] = p_value;
}

void mouse::update_wheel(int p_x, int p_y)
{
	m_current_wheel_x += p_x; m_current_wheel_y += p_y;
}

void mouse::update_motion(int p_x, int p_y)
{
	m_current_x = p_x; m_current_y = p_y;
}

int mouse::get_motion_x()
{
	return m_current_x - m_last_x;
}

int mouse::get_motion_y()
{
	return m_current_y - m_last_y;
}

int mouse::get_position_x()
{
	return m_current_x;
}

int mouse::get_position_y()
{
	return m_current_y;
}

int mouse::get_wheel_motion_x()
{
	return m_current_wheel_x - m_last_wheel_x;
}

int mouse::get_wheel_motion_y()
{
	return m_current_wheel_y - m_last_wheel_y;
}

int mouse::get_wheel_position_x()
{
	return m_current_wheel_x;
}

int mouse::get_wheel_position_y()
{
	return m_current_wheel_y;
}
