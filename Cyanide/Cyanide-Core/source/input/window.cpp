// window.cpp

#include "input/window.h"
#include <SDL.h>

void window_info::set_window(SDL_Window* p_window)
{
	m_window = p_window;
	refresh_window_size();
}

bool window_info::resized()
{
	return m_current_resized;
}

void window_info::refresh_window_size()
{
	SDL_GetWindowSize(m_window, &m_w, &m_h);
	if(m_w % 8)
		m_w = m_w - (m_w % 8);
	if(m_h % 8)
		m_h = m_h - (m_h % 8);
	SDL_SetWindowSize(m_window, m_w, m_h);
}

void window_info::reset()
{
	m_current_resized = false;
}

void window_info::update_resize(bool p_resized)
{
	if(p_resized)
		refresh_window_size();
	m_current_resized = p_resized;
}

bool window_info::is_set()
{
	return m_window;
}

int window_info::window_w()
{
	return m_w;
}

int window_info::window_h()
{
	return m_h;
}
