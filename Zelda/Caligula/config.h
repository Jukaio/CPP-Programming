// config.h

#ifndef INCLUDED_CONFIG_H
#define INCLUDED_CONFIG_H

#include <SDL_rect.h>
#include <SDL_scancode.h>

// Config data
// Controls 
// State/Scene specific data for texture source information and
// game specific destinations (such as spawn points)

namespace CONFIG
{
	const int SCALE = 4;
	const int TILE_SIZE = 16;
	const int UNSCALED_SCREEN_WIDTH = 256;
	const int UNSCALED_SCREEN_HEIGHT = 224;
	const int SCREEN_WIDTH = UNSCALED_SCREEN_WIDTH * SCALE;
	const int SCREEN_HEIGHT = UNSCALED_SCREEN_HEIGHT * SCALE;

};

namespace CONTROLS
{
	const SDL_Scancode UP_BOTTON = SDL_SCANCODE_W;
	const SDL_Scancode DOWN_BOTTON = SDL_SCANCODE_S;
	const SDL_Scancode LEFT_BOTTON = SDL_SCANCODE_A;
	const SDL_Scancode RIGHT_BOTTON = SDL_SCANCODE_D;

	const SDL_Scancode START_BUTTON = SDL_SCANCODE_SPACE;
	const SDL_Scancode SELECT_BUTTON = SDL_SCANCODE_BACKSPACE;
}

namespace TITLE_SCREEN
{
	const SDL_Rect BG_SRC_RCT = { 1, 11,	CONFIG::UNSCALED_SCREEN_WIDTH, CONFIG::UNSCALED_SCREEN_HEIGHT };
	const SDL_Rect BG_DST_RCT = { 0, 0,	CONFIG::UNSCALED_SCREEN_WIDTH, CONFIG::UNSCALED_SCREEN_HEIGHT };
	const int MUSIC_FADE_IN = 5000;
	const float TIMER = 5.0f * 60.0f;
}

namespace LORE_SCREEN
{
	const int PAGES_COLUMNS = 4;
	const int PAGES_ROWS = 2;
	const SDL_Rect BASE_SRC_RCT =  { 1, 250,	CONFIG::UNSCALED_SCREEN_WIDTH, CONFIG::UNSCALED_SCREEN_HEIGHT };
	const SDL_Rect BASE_DST_RECT = { 0, 0,		CONFIG::UNSCALED_SCREEN_WIDTH, CONFIG::UNSCALED_SCREEN_HEIGHT };
	const int SRC_OFFSET = 1;
	const int SRCOLL_SPEED_X = 0;
	const int SCROLL_SPEED_Y = -1;
	const int SCROLL_STOP = 161;
}

namespace MENU_SCREEN
{
	// Numeric Values
	const int NUMBER_OF_SAVE_CLOTS = 3;

	// could do this as an enum, but that works just fine
	const int NO_SELECTION = -1;
	const int FIRST_SAVE = 0;
	const int SECOND_SAVE = 1;
	const int THIRD_SAVE = 2;
	const int REGISTRATION = 3;
	const int ELIMINATION = 4;

	const SDL_Rect BG_SRC_RCT					= { 1, 1,	CONFIG::UNSCALED_SCREEN_WIDTH, CONFIG::UNSCALED_SCREEN_HEIGHT };
	const SDL_Rect BG_DST_RCT					= { 0, 0,	CONFIG::UNSCALED_SCREEN_WIDTH, CONFIG::UNSCALED_SCREEN_HEIGHT };

	// Source Rects
	const SDL_Rect HEART_CURSOR_SRC_RCT			= { 73, 247, 8, 8 };
	const SDL_Rect LINK_SRC_RCT					= { 1, 230, 16, 16 };
	const SDL_Rect SWORD_SRC_RCT				= { 64, 247, 8, 16 };
	const SDL_Rect LIFE_HEART_SRC_RCT			= { 52, 230, 8, 8 };
	const SDL_Rect INACTIVE_FIELD_SRC_RCT		= { 86, 240, 8, 8 };

	const SDL_Rect BG_LINK_SRC_RCT				= { 1, 200, 16, 16 };
	const SDL_Rect BG_SWORD_SRC_RCT				= { 64, 200, 8, 16 };

	const SDL_Rect BLACK_BACKGROUND_SRC_RCT		= { 73, 65, 64, 16 };

	// Destination Rects
	const SDL_Rect LINK_DST_RCT					= { 48, 81, 16, 16 };
	const SDL_Rect SWORD_DST_RCT				= { 60, 78, 8, 16 };

	const SDL_Rect CURSOR_ORIGIN_DST_RCT		= { 40, 85, 8, 8 };
	const SDL_Rect SAVE_SLOT_OFFSET_Y_DST_RCT	= { 0, 24, 0, 0 };

	const SDL_Rect LIFE_HEART_ORIGIN_DST_RCT	= { 144, 80, 8, 8 };
	const SDL_Rect LIFE_HEART_OFFSET_X_DST_RCT	= { 8, 0, 0, 0 };
	const SDL_Rect LIFE_HEART_OFFSET_Y_DST_RCT	= { 0, 8, 0, 0 };
	const int LIFE_HEART_ROW_COUNT = 8;
	const int LIFE_HEART_COL_COUNT = 2;

	const SDL_Rect NAME_BACKGROUND_DST_RCT		= { 72, 80, 64, 16 };
	const SDL_Rect LIFE_BACKGROUND_DST_RCT		= { 144, 80, 64, 16 };

	const SDL_Rect REGISTRATION_DST_RCT			= { 40, 161, 8, 8 };
	const SDL_Rect ELIMINATION_DST_RCT			= { 40, 177, 8, 8 };
}

namespace GAMEPLAY_SCREEN
{
	const int AMOUNT_OF_LEVELS = 3;

	const int UNDERWORLD_BLUE_ROWS = 5;
	const int UNDERWORLD_BLUE_COLUMNS = 8;

	// Source Rects
	const SDL_Rect LINK_DOWN_SRC_RCT = { 1, 11, 16, 16 };
	const SDL_Rect LINK_RIGHT_SRC_RCT = { 35, 11, 16, 16 };
	const SDL_Rect LINK_UP_SRC_RCT = { 69, 11, 16, 16 };
	const SDL_Rect SWORD_UP_SRC = { 36, 154, 8, 16 };
	const SDL_Rect SWORD_RIGHT_SRC = { 45, 154, 16, 16 };

	const SDL_Rect OCTOROK_SRC_RCT = { 18, 11, 16, 16 };
	const SDL_Rect OCTOROK_BULLET_SRC_RCT = { 69, 11, 8, 16 };

	// Destination Rects
	const SDL_Rect LINK_SPAWN_DST_RCT = { 100, 100, 16, 16 };

	// Statistical values
	const int LINK_MAX_HP = 3;
	const float LINK_ATTACK_STAGGER = 0.5f;
	// Other things

	// Source Rects
	const SDL_Rect HP_PICK_UP_SRC_RCT = { 299, 185, 8, 16 };

	// Destination Rects
	const int HP_PICK_UP_AMOUNT = 3;
	const int LEVEL_OFFSET[AMOUNT_OF_LEVELS] = { CONFIG::UNSCALED_SCREEN_WIDTH * 0,
												 CONFIG::UNSCALED_SCREEN_WIDTH * 1,
												 CONFIG::UNSCALED_SCREEN_WIDTH * 2};
	const SDL_Rect HP_PICK_UP_SPAWNS[HP_PICK_UP_AMOUNT] = { { 124 + LEVEL_OFFSET[0], 96 , 8, 16 },
															{ 124 + LEVEL_OFFSET[1], 96 , 8, 16 },
															{ 124 + LEVEL_OFFSET[2], 96 , 8, 16 }
	};
	const int ENEMY_AMOUNT = 7;
	const SDL_Rect ENEMY_SPAWNS[ENEMY_AMOUNT] = { { 140 + LEVEL_OFFSET[0], 120, 16, 16 },
												  { 40	+ LEVEL_OFFSET[0], 40, 16, 16 },
												  { 144	+ LEVEL_OFFSET[1], 40, 16, 16 },
												  { 144	+ LEVEL_OFFSET[1], 124, 16, 16 },
												  { 80	+ LEVEL_OFFSET[2], 54, 16, 16 },
												  { 160	+ LEVEL_OFFSET[2], 72, 16, 16 },
												  { 124	+ LEVEL_OFFSET[2], 140, 16, 16 }
	};
}


#endif // !INCLUDED_CONFIG_H