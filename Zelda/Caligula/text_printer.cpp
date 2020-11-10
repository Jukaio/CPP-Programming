// printer.cpp

#include "text_printer.h"
#include <SDL_image.h>
#include "texture.h"
#include "text_map_format.h"
#include "config.h"

text_printer::text_printer(SDL_Renderer* p_renderer)
	: m_renderer(p_renderer)
{
}

bool text_printer::load(std::string p_id, std::string p_string, texture* p_texture, text_map_format* p_text_format)
{
	if(p_texture == nullptr)
		return false;
	text* temp = new text(p_string, p_texture, p_text_format);
	m_cache.add(p_id, temp);
	return true;
}

bool text_printer::unload(std::string p_id)
{
	return m_cache.remove(p_id);
}

void text_printer::change_string(std::string p_id, std::string new_string)
{
	m_cache[p_id]->set_string(new_string);
}

bool text_printer::draw(std::string p_id, SDL_Rect p_bounds, int p_scale)
{
	text* temp = m_cache[p_id];
	if(temp == nullptr)
		return false;
		
	const char* c = &temp->get_string()->front();
	text_map_format format = *temp->get_text_map_format();
	int size_x = format.get_character_size().x * p_scale;
	int size_y = format.get_character_size().y * p_scale;

	int x = 0;
	int y = 0;
	while(*c != '\0')
	{
		if(*c == '\n')
		{
			x = 0;
			y++;
		}
		else
		{
			SDL_Rect source = create_source_rect_through_char(*c, format);

			int position_x = p_bounds.x + (x * size_x);
			int position_y = p_bounds.y + (y * size_y);
			SDL_Rect dest = { position_x,
							  position_y,
							  size_x,
							  size_y };

			SDL_RenderCopy(m_renderer, temp->get_texture()->get(), 
						   &source, &dest);

			if(x >= p_bounds.w / format.get_character_size().x) { x = 0; y++; }
			else x++;

			// Checks whether the text is printing out of the bounding box or not.
			// I have no idea. It is not significant enough to return false, but also not 
			// insignificant enough to return true - Fuck it! It will be very obvious in the game
			// whether it is out of bounds or not, therefore it returns true!
			if(y >= p_bounds.h / format.get_character_size().y) 
				return true;
		}
		c++;
	}
	return true;
}

SDL_Rect text_printer::create_source_rect_through_char(const char p_c, text_map_format p_format)
{
	int x, y;
	// hardcoded positional IDs, because the Zelda bitmap is not based on the typical unicode or ascii order
	// something like that could get implemented in text_map_format
	// but that will do for now
	if(p_c >= '0' && p_c <= '9')
	{
		int index = p_c - ' ';
		x = index % p_format.get_dimension().x;
		y = index / p_format.get_dimension().x - 1;
	}
	else if(p_c >= 'A' && p_c <= 'Z')
	{
		int index = p_c - ' ' - 7;
		x = index % p_format.get_dimension().x;
		y = index / p_format.get_dimension().x - 1;
	}
	else if(p_c >= 'a' && p_c <= 'z')
	{
		int index = p_c - ' ' - (7 + 32);
		x = index % p_format.get_dimension().x;
		y = index / p_format.get_dimension().x - 1;
	}
	else if(p_c == ' ')
	{
		x = 12;
		y = 2;
	}
	else
	{
		x = 11;
		y = 2;
	}

	int offset_x = p_format.get_character_size().x + p_format.get_character_spacing().x;
	int offset_y = p_format.get_character_size().y + p_format.get_character_spacing().y;

	return { p_format.get_sheet_position().x + (x * offset_x), 
			 p_format.get_sheet_position().y + (y * offset_y),
			 p_format.get_character_size().x, 
			 p_format.get_character_size().y };
}
