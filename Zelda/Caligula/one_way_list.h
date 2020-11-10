// one_way_list_selection.h

#ifndef INCLUDED_ONE_WAY_LIST_SELECTION_H
#define INCLUDED_ONE_WAY_LIST_SELECTION_H


#include "entity.h"
#include <vector>
#include "selection.h"
#include <SDL_scancode.h>

// Class which manages multiple selections
// Adds a "selector", an index, which checks what is currently hovered
// Also checks inputs

struct one_way_list : entity
{
	one_way_list(SDL_Scancode p_select, SDL_Scancode p_start);
	~one_way_list();

	void add_selection(selection* p_selection);
	
	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;

	int get_selected_index_if_confirmed();

private:
	std::vector<selection*> m_selections;
	selection* m_curr_selected = nullptr;
	selection* m_prev_selected = nullptr;

	bool m_first_initialised = false;
	SDL_Scancode m_select;
	SDL_Scancode m_start;
	int m_index = -1;

	int get_first_selectable();

};

#endif // !INCLUDED_ONE_WAY_LIST_SELECTION_H



