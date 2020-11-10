// text_map_format.h

#ifndef INCLUDED_TEXT_MAP_FORMAT_H
#define INCLUDED_TEXT_MAP_FORMAT_H

// holds the significant data to get the right areas (position, padding, spacing, etc.) of the characters
// a typedef struct would have worked the same way, but it looks prettier with methods in my opinion

struct SDL_Point;

struct text_map_format
{
	text_map_format(int p_sheet_position_x,
					int p_sheet_position_y,
					int p_spacing_x,
					int p_spacing_y,
					int p_characters_per_row,
					int p_characters_per_column,
					int p_character_size_x,
					int p_character_size_y);

	SDL_Point get_sheet_position();
	SDL_Point get_character_spacing();
	SDL_Point get_dimension();
	SDL_Point get_character_size();

private:
	int m_sheet_x, m_sheet_y		= 0;
	int m_spacing_x, m_spacing_y	= 0;
	int m_row_length				= 0;
	int m_column_length				= 0;
	int m_character_size_w			= 0;
	int m_character_size_h			= 0;
};

#endif // !INCLUDED_TEXT_MAP_FORMAT_H



