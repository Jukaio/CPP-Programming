// keyboard.cpp

#include "input/keyboard.h"

keyboard::keyboard()
	: m_current_frame((size_t)key::NUM_KEYS, false)
	, m_previous_frame((size_t)key::NUM_KEYS, false)
{

}

keyboard::~keyboard()
{
}

void keyboard::update()
{
	m_previous_frame = m_current_frame;
}

bool keyboard::key_down(short int p_key)
{
    return m_current_frame[p_key];
}

bool keyboard::key_pressed(short int p_key)
{
	return m_current_frame[p_key] && !m_previous_frame[p_key];
}

bool keyboard::key_released(short int p_key)
{
	return !m_current_frame[p_key] && m_previous_frame[p_key];
}

void keyboard::update_key(short int p_key, bool p_value)
{
	m_current_frame[p_key] = p_value;
}
