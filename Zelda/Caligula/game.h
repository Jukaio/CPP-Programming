// game.h

#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

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

#include "finite_state_machine.h"


// A base class for any kind of game.
// This class will provide the basic engine functionality.
// Anything what is supposed to get added engine wise, 
// Each derived class has to call the constructor of this base class so it creates and initialises all services

struct SDL_Renderer;

struct game
{
	game(SDL_Renderer* p_renderer);

	// Pure virtual functions, so the derived struct/calss is forced to implement those
	virtual void enter() = 0;
	virtual void run() = 0;
	virtual void exit() = 0;

	input_handler m_input_handler;					// Handles input (Woah!)
	collision_handler m_collisions_handler;			// Checks collisions
	sound_container m_sound_container;				// Load/Unload and get sounds
	music_player m_music_player;					// Load/Unload, play/stop/resume music
	texture_manager m_textures_manager;				// Manages textures - Maybe I put more functionality in it TODO: draw functions
	sprite_drawer m_sprite_container;				// Load/Unload and get sprites
	text_map_format_atlas m_text_map_format_atlas;	// An atlas containing different keyboard formats (char to a source rectangle for sprite text)
	text_printer m_text_printer;					// Load/Unload and prints text

	finite_state_machine m_finite_state_machnine;	// Deals with the different game states										

protected:
	SDL_Renderer* m_renderer = nullptr;				
	bool m_running = false;

};

#endif // !INCLUDED_GAME_H



