// state.h

#ifndef INCLUDED_STATE_H
#define INCLUDED_STATE_H

#include "service.h"
#include "config.h"

#include "collision_handler.h"
#include "input_handler.h"
#include "sprite_drawer.h"
#include "texture_manager.h"
#include "sound_container.h"
#include "music_player.h"
#include "text_printer.h"
#include "text_map_format_atlas.h"

#include <vector>
#include <string>

// This state base class/struct provides everything needed 
// to create a state in regards of updating, rendering and resource "management"

struct entity;

struct state
{
	state(std::string p_name);
	virtual ~state();

	virtual void enter() {};
	virtual bool update(int p_ms) = 0; //Should return false if state should be changed
	virtual void render() = 0;
	virtual void exit() {};

	std::string get_next_state();
	std::string get_name();

protected:
	std::string m_name;
	std::string m_next_state;

	// List of all entities
	// First push_back gets rendered first
	// Last gets rendered last
	// Could implement something like a sorting list so it would
	// sort everything based on a layer index
	std::vector<entity*> m_entities;

	// Some shortcuts to the services - My wrists will thank me later
	// edit: My wrists did thank me later
	input_handler* m_inputs = nullptr;
	collision_handler* m_collisions = nullptr;
	sound_container* m_sounds = nullptr;
	music_player* m_music = nullptr;
	texture_manager* m_textures = nullptr;
	sprite_drawer* m_sprites = nullptr;
	text_map_format_atlas* m_text_maps = nullptr;
	text_printer* m_printer = nullptr;
};

#endif // !INCLUDED_STATE_H
