// one_way_list_selection.cpp

#include "one_way_list.h"
#include "service.h"
#include "input_handler.h"

one_way_list::one_way_list(SDL_Scancode p_select, SDL_Scancode p_start)
	: m_select(p_select)
	, m_start(p_start)
	, m_first_initialised(false)
{

}

one_way_list::~one_way_list()
{
	for(auto&& selection : m_selections)
	{
		if(selection != nullptr)
			delete selection;
		selection = nullptr;
	}
	m_curr_selected = nullptr;
	m_prev_selected = nullptr;
}

void one_way_list::add_selection(selection* p_selection)
{
	m_selections.push_back(p_selection);
	if(!m_first_initialised && p_selection->is_selectable())
	{
		m_index = get_first_selectable();
		m_first_initialised = true;

		m_curr_selected = m_selections[m_index];
		m_curr_selected->set_selected(true);
	}
}

void one_way_list::on_enable()
{
}

void one_way_list::update(int p_ms)
{
	if(service<input_handler>::get()->key_pressed(m_select))
	{
		m_index++;
		int size = m_selections.size();
		if(m_index >= size)
		{
			m_index = get_first_selectable();
			m_prev_selected = m_curr_selected;
			m_curr_selected = m_selections[m_index];
		}
		else
		{
			m_prev_selected = m_curr_selected;
			m_curr_selected = m_selections[m_index];
		}

		m_prev_selected->set_selected(false);
		m_curr_selected->set_selected(true);
	}
}

void one_way_list::render()
{
	for(auto&& selection : m_selections)
		selection->render();
}

void one_way_list::on_disable()
{
}

void one_way_list::on_collision(entity* p_other)
{
}

int one_way_list::get_selected_index_if_confirmed()
{
	if(service<input_handler>::get()->key_pressed(m_start))
	{
		return m_index;
	}
	return -1; // if m_start isnt't pressed, it returns -1;
}

int one_way_list::get_first_selectable()
{
	for(int i = 0; i < m_selections.size(); i++)
	{
		if(m_selections[i]->is_selectable())
			return i;
	}
	return -1;
}

void one_way_list::reset()
{
}
