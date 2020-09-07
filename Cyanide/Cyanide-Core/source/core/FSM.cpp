// FSM.cpp

#include "core/FSM.h"
#include "core/state.h"
#include <SDL.h>
#include <assert.h>

FSM::FSM()
	: m_current(nullptr)
{
}

FSM::~FSM()
{
}

void FSM::set_renderer(SDL_Renderer* p_renderer)
{
	m_renderer = p_renderer;
}

void FSM::run(int p_ms)
{
	if(!m_current)
		return;

	bool swap = !m_current->core_update(p_ms);
	m_current->core_render(m_renderer);
	if(swap)
		set_state(m_current->get_next());
}

void FSM::add_state(state* p_state)
{
	if(p_state)
		m_cache.add(p_state->get_id(), p_state);
}

void FSM::remove_state(int p_id)
{
	if(m_cache.exists(p_id))
		m_cache.remove(p_id);
}

void FSM::set_state(int p_id)
{
	if(m_cache.exists(p_id))
	{
		if(m_current)
			m_current->exit();
		m_current = m_cache.get(p_id);
		m_current->enter(m_renderer);
		return;
	}
	assert(!("Could not switch from %d to %d", m_current->get_id(), p_id));
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not switch from %d to %d", m_current->get_id(), p_id);
}

void FSM::shut_down()
{
	if(m_current)
		m_current->exit();
}

