// text_map_format.cpp

#include "text_map_format.h"
#include <SDL.h>

text_map_format::text_map_format(int p_sheet_position_x, int p_sheet_position_y, 
								 int p_spacing_x, int p_spacing_y, 
								 int p_characters_per_row, int p_characters_per_column, 
								 int p_character_size_w, int p_character_size_h)
	: m_sheet_x(p_sheet_position_x)
	, m_sheet_y(p_sheet_position_y)
	, m_spacing_x(p_spacing_x)
	, m_spacing_y(p_spacing_y)
	, m_row_length(p_characters_per_row)
	, m_column_length(p_characters_per_column)
	, m_character_size_w(p_character_size_w)
	, m_character_size_h(p_character_size_h)
{
}

SDL_Point text_map_format::get_sheet_position()
{
	return SDL_Point({m_sheet_x, m_sheet_y});
}

SDL_Point text_map_format::get_character_spacing()
{
	return SDL_Point({m_spacing_x, m_spacing_y});
}

// x = row
// y = column - this one practically gets never used, but I keep it that way for coherency
SDL_Point text_map_format::get_dimension()
{
	return SDL_Point({m_row_length, m_column_length});
}

SDL_Point text_map_format::get_character_size()
{
	return SDL_Point({m_character_size_w, m_character_size_h});
}
