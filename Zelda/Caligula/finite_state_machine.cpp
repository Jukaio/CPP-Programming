// finite_state_machine.cpp

#include "finite_state_machine.h"
#include "state.h"
#include <SDL_log.h>
#include <SDL_error.h>

finite_state_machine::finite_state_machine() : m_current_state(nullptr)
{
}

finite_state_machine::~finite_state_machine()
{
}

// Switches from one state to another
void finite_state_machine::switch_state(std::string p_state)
{
	state* next_state = m_cache[p_state];
	if(next_state != nullptr)
	{
		if(m_current_state != nullptr)
			m_current_state->exit();				
		m_current_state = next_state;				
		m_current_state->enter();					
		return;
	}
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not switch from %s to %s", m_current_state->get_name().c_str(),  p_state.c_str());
}

void finite_state_machine::update(int p_ms)
{
	if (m_current_state == nullptr)
		return;

	// The state switch is implemented this way to render the very last frame before switching,
	// otherwise it would skip the last frame render - I didnt try what would happen otherwise, 
	// but I just imagine it would provoke a "less clean" visual transition
	bool switching = !m_current_state->update(p_ms);
	m_current_state->render();

	if (switching == true) // Switch state
		switch_state(m_current_state->get_next_state());
}

void finite_state_machine::add_state(state* p_state)
{
	if (p_state == nullptr)
		return;

	m_cache.add(p_state->get_name(), p_state);
}

void finite_state_machine::remove_state(std::string p_state)
{
	if (p_state == "")
		return;

	m_cache.remove(p_state);
}

void finite_state_machine::set_state(std::string p_state)
{
	state* next_state = m_cache[p_state];
	if(next_state != nullptr)
	{
		if(m_current_state != nullptr)
			m_current_state->exit();
		m_current_state = next_state;
		m_current_state->enter();
		return;
	}
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not set state: %s", p_state);
}
