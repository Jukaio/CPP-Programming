// menu_state.cpp

#include "menu_state.h"
#include "static_image.h"
#include "entity.h"
#include "config.h"
#include <iostream>
#include "sdl_math.h"
#include "one_way_list.h"

using namespace MENU_SCREEN;

menu_state::menu_state(std::string p_name)
	: state(p_name)
{
	// Texture
	texture* menu_texture = m_textures->get("menu");
	
	// Used sprites
	sprite* background			= m_sprites->load_and_get("menu_background", menu_texture, BG_SRC_RCT);
	sprite* active_heart_cursor = m_sprites->load_and_get("heart_cursor", menu_texture, HEART_CURSOR_SRC_RCT);
	sprite* link				= m_sprites->load_and_get("link", menu_texture, LINK_SRC_RCT);
	sprite* sword				= m_sprites->load_and_get("sword", menu_texture, SWORD_SRC_RCT);
	sprite* life				= m_sprites->load_and_get("life_heart", menu_texture, LIFE_HEART_SRC_RCT);
	sprite* inactive_field		= m_sprites->load_and_get("inactive_8x8", menu_texture, INACTIVE_FIELD_SRC_RCT);
	sprite* bg_link				= m_sprites->load_and_get("bg_link", menu_texture, BG_LINK_SRC_RCT);
	sprite* bg_sword			= m_sprites->load_and_get("bg_sword", menu_texture, BG_SWORD_SRC_RCT);
	sprite* black_background	= m_sprites->load_and_get("inactive_64x16", menu_texture, BLACK_BACKGROUND_SRC_RCT);

	// Add the background to the vector
	m_entities.push_back(new static_image(background, BG_DST_RCT));

	// Create an instance of a one way list selection 
	m_menu_selection = new one_way_list(CONTROLS::SELECT_BUTTON, CONTROLS::START_BUTTON);

	// Each save slot has a certain set of sprites
	for(int i = 0; i < NUMBER_OF_SAVE_CLOTS; i++)
	{
		selection* save_slot = new selection(true, static_image(active_heart_cursor, CURSOR_ORIGIN_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(black_background, LIFE_BACKGROUND_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(black_background, NAME_BACKGROUND_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(inactive_field, CURSOR_ORIGIN_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(bg_link, LINK_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(bg_sword, SWORD_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(sword, SWORD_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));
		save_slot->add_image(static_image(link, LINK_DST_RCT + (SAVE_SLOT_OFFSET_Y_DST_RCT * i)));

		for(int y = 0; y < LIFE_HEART_COL_COUNT; y++)
		{
			for(int x = 0; x < LIFE_HEART_ROW_COUNT; x++)
			{
				SDL_Rect draw_rect = LIFE_HEART_ORIGIN_DST_RCT +
									(LIFE_HEART_OFFSET_X_DST_RCT * x) +
									(LIFE_HEART_OFFSET_Y_DST_RCT * y) +
									(SAVE_SLOT_OFFSET_Y_DST_RCT * i);

				save_slot->add_image(static_image(life, draw_rect));
			}
		}
		m_menu_selection->add_selection(save_slot);
	}

	// Registrations menu selection
	selection* reg =  new selection(true, static_image(active_heart_cursor, REGISTRATION_DST_RCT));
	reg->add_image(static_image(inactive_field, REGISTRATION_DST_RCT));
	m_menu_selection->add_selection(reg);

	// Elimination menu selection
	selection* eli = new selection(true, static_image(active_heart_cursor, ELIMINATION_DST_RCT));
	eli->add_image(static_image(inactive_field, ELIMINATION_DST_RCT));
	m_menu_selection->add_selection(eli);

	// Add menu_selection to the vector
	m_entities.push_back(m_menu_selection);
}

void menu_state::enter()
{
	
}

bool menu_state::update(int p_ms)
{
	for(auto&& e : m_entities)
	{
		if(e->is_active())
			e->update(p_ms);
	}

	int index = m_menu_selection->get_selected_index_if_confirmed();
	switch(index)
	{
		case NO_SELECTION:
			break;
		case FIRST_SAVE:
		case SECOND_SAVE:
		case THIRD_SAVE:
			m_next_state = "gameplay";
			return false;;
		case REGISTRATION:
			m_next_state = "registration";
			return false;
		case ELIMINATION:
			m_next_state = "elimination";
			return false;
	}

	return true;
}

void menu_state::render()
{
	for(auto&& e : m_entities)
	{
		if(e->is_active() && e->is_visible())
			e->render();
	}
}

void menu_state::exit()
{
	m_music->stop();
}
