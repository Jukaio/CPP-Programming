// gameplay_state.h

#ifndef INCLUDED_GAMEPLAY_STATE_H
#define INCLUDED_GAMEPLAY_STATE_H

#include "state.h"

struct tile_set;
struct layer;
struct player;
struct octorok;
struct sword;
struct health_pickup;
struct projectile;
struct GUI;

struct gameplay_state : state
{
	gameplay_state(std::string p_name);
	~gameplay_state();
	virtual void enter() final;
	virtual bool update(int p_ms) final;
	virtual void render() final;
	virtual void exit() final;

private:
	tile_set* m_tile_set; 
	player* m_link;
	sword* m_sword;
	GUI* m_gui;
	std::vector<layer*> m_layers;
	std::vector<octorok*> m_enemies;
	std::vector<health_pickup*> m_health_pickups;
	std::vector<projectile*> m_bullets;
};

#endif //!INCLUDED_GAMEPLAY_STATE_H
