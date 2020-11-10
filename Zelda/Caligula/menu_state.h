// menu_state.h

#ifndef INCLUDED_MENU_STATE_H
#define INCLUDED_MENU_STATE_H

#include "state.h"

struct one_way_list;

struct menu_state : state
{
	menu_state(std::string p_name);
	virtual void enter() final;
	virtual bool update(int p_ms) final;
	virtual void render() final;
	virtual void exit() final;

private:
	one_way_list* m_menu_selection;
	int m_index_to_render = 1;
};

#endif // !INCLUDED_MENU_STATE_H



