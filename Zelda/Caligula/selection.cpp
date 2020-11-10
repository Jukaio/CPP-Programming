// selection.cpp

#include "selection.h"

selection::selection(bool p_selectable, static_image p_hover)
	: m_selectable(p_selectable)
	, m_hover_image(p_hover)
	, m_selected(false)
{
}

void selection::on_enable()
{
}

void selection::update(int p_ms)
{
}

void selection::render()
{
	multiple_images::render();
	if(m_selected)
		m_hover_image.render();
}

void selection::on_disable()
{
}

void selection::on_collision(entity* p_other)
{
}

bool selection::is_selectable()
{
	return m_selectable;
}

void selection::set_selectable(bool p_selectable)
{
	m_selectable = p_selectable;
}

bool selection::is_selected()
{
	return m_selected;
}

void selection::set_selected(bool p_selected)
{
	m_selected = p_selected;
}
