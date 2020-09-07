// mouse.cpp

#include "input/mouse.h"

mouse::mouse()
    : m_current_frame((size_t)button::NUM_KEYS, false)
    , m_previous_frame((size_t)button::NUM_KEYS, false)
    , m_current_pos(vector2::zero)
    , m_last_pos(vector2::zero)
    , m_current_wheel(vector2::zero)
    , m_last_wheel(vector2::zero)
{

}

mouse::~mouse()
{
}

void mouse::update()
{
    m_previous_frame = m_current_frame;
    m_last_pos = m_current_pos;
    m_last_wheel = m_current_wheel;
}

bool mouse::button_down(short p_button)
{
    if(p_button >= (size_t)button::NUM_KEYS)
        return false;

    return m_current_frame[p_button];
}

bool mouse::button_pressed(short p_button)
{
    if(p_button >= (size_t)button::NUM_KEYS)
        return false;

    return (m_previous_frame[p_button] == false && m_current_frame[p_button] == true);
}

bool mouse::button_released(short p_button)
{
    if(p_button >= (size_t)button::NUM_KEYS)
        return false;

    return (m_previous_frame[p_button] == true && m_current_frame[p_button] == false);
}

void mouse::update_button(short p_button, bool p_value)
{
    if(p_button >= (size_t)button::NUM_KEYS)
        return;
    m_current_frame[p_button] = p_value;
}

void mouse::update_wheel(int p_x, int p_y)
{
    m_current_wheel.x = p_x;
    m_current_wheel.y = p_y;
}

void mouse::update_motion(int p_x, int p_y)
{
    m_current_pos.x = p_x; 
    m_current_pos.y = p_y;
}

vector2 mouse::get_motion()
{
    return m_current_pos - m_last_pos;
}

vector2 mouse::get_position()
{
    return m_current_pos;
}

vector2 mouse::get_wheel_motion()
{
    return m_current_wheel - m_last_wheel;
}

vector2 mouse::get_wheel_position()
{
    return m_current_wheel;
}
