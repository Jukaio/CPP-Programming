// zelda.cpp

#include "zelda.h"
#include <SDL.h>

#include "title_state.h"
#include "lore_state.h"
#include "menu_state.h"
#include "gameplay_state.h"
#include "win_state.h"
#include "lose_state.h"
#include "camera.h"

zelda::zelda(SDL_Renderer* p_renderer)
	: game(p_renderer)
{
}

void zelda::enter()
{
	// Setting a colour key to get transparency 
	m_textures_manager.set_alpha_channel_color(116, 116, 116);

	// Pre-loads some resources
	// I would prefer to only pre-load textures, font maps and music in here
	// Sound effects, sprites are more scene specific, thus it makes more hierarchical sense
	// to load them in the states/scenes
	m_music_player.load("title", "../Assets/title_state/title_screen.mp3");
	m_music_player.load("play", "../Assets/gameplay_state/play.mp3");
	m_music_player.load("lose", "../Assets/gameplay_state/lose.mp3");
	m_music_player.load("win", "../Assets/gameplay_state/win.mp3");

	m_textures_manager.load("title_lore", "../Assets/title_state/title_screen.png");
	m_textures_manager.load("menu", "../Assets/menu_state/menu.png");
	m_textures_manager.load("underworld_blue", "../Assets/gameplay_state/underworld_blue.png");
	m_textures_manager.load("link", "../Assets/gameplay_state/link.png");
	m_textures_manager.load("enemies", "../Assets/gameplay_state/31805.png");

	m_textures_manager.load("Environment", "../Assets/gameplay_state/8375.png");
	m_textures_manager.load("font", "../Assets/font.png");

	// Text map formats / "Keyboards" (you know. the letters)
	// the text_map_format just holds a lot of values
	// could have used a typedef instead
	m_text_map_format_atlas.load("white", new text_map_format(336, 24, 8, 8, 16, 3, 8, 8));
	m_text_map_format_atlas.load("blue", new text_map_format(336, 88, 8, 8, 16, 3, 8, 8));
	m_text_map_format_atlas.load("red", new text_map_format(336, 152, 8, 8, 16, 3, 8, 8));
	m_text_map_format_atlas.load("green", new text_map_format(336, 216, 8, 8, 16, 3, 8, 8));

	// This section creates/initialises the states
	title_state* title = new title_state("title");
	lore_state* lore = new lore_state("lore");
	menu_state* menu = new menu_state("menu");
	gameplay_state* gameplay = new gameplay_state("gameplay");
	lose_state* lose = new lose_state("lose");
	win_state* win = new win_state("win");

	// This section adds the states to the state machine
	m_finite_state_machnine.add_state(title);
	m_finite_state_machnine.add_state(lore);
	m_finite_state_machnine.add_state(menu);
	m_finite_state_machnine.add_state(gameplay);
	m_finite_state_machnine.add_state(lose);
	m_finite_state_machnine.add_state(win);

	// Sets the first state or "root state"
	m_finite_state_machnine.set_state(title->get_name());
}

void zelda::run()
{
	// Switches the core loop on, if running becomes false the program quits 
	m_running = true;

	// Initialises the needed variables to receive delta time / frame time
	auto tp = SDL_GetTicks();			// initialises tp with the first time time based on the total SDL ticks
	int dt = SDL_GetTicks() - tp;		// Initialises dt with the first calculation of dt (this will be the same
										// as tp, since it gets calculated right after)

	const int FPS = 60;
	const int FTPS = (1.0f/FPS) * 1000;	// Calculates the wanted time per frame

	while(m_running)
	{
		tp = SDL_GetTicks();								// get time point
									
		m_input_handler.handle_events();					// Updates the input events
		if(m_input_handler.key_down(SDL_SCANCODE_ESCAPE))	// If "ESC" was pressed, stop the program
			m_running = false;

		// Clear screen
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
		SDL_RenderClear(m_renderer);

		// Update finite state machine
		m_finite_state_machnine.update(dt); 

		// Present final frame render screen 
		SDL_RenderPresent(m_renderer);

		dt = SDL_GetTicks() - tp;							// Receive delta time from this frame
		int delay_time = FTPS - dt;							
		delay_time = (delay_time < 0) ? 0 : delay_time;		// If delta_time is bigger than the frame_time, we do not want to delay
		SDL_Delay(delay_time);								// Delay until the total wanted frame time is reached 
		
		dt += delay_time;									// Add the delay to dt (total frame time)
	}
}

void zelda::exit()
{
	
}

