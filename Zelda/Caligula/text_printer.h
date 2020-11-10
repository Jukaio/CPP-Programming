// printer.h

#ifndef INCLUDED_PRINTER_H
#define INCLUDED_PRINTER_H

#include "resource_cache.h"
#include "text.h"

// draws and changes a pre-loaded string
// besides that it holds a collection/cache of all the text

struct SDL_Renderer;
struct text_map_format;
struct SDL_Rect;

struct text_printer
{
	text_printer(SDL_Renderer* p_renderer);
	bool load(std::string p_id, std::string p_string, texture* p_texture, text_map_format* p_text_format);
	bool unload(std::string p_id);

	// Change the string of a text (stored through a string ID) to another string
	void change_string(std::string p_id, std::string new_string);

	// Draws the text with the specific id in the specified area with the specified bounds (can get scaled too)
	bool draw(std::string p_id, SDL_Rect p_bounds, int p_scale = 1);

private:
	SDL_Renderer* m_renderer;
	resource_cache<text> m_cache;

	SDL_Rect create_source_rect_through_char(char p_c, text_map_format p_format);
};

#endif // !INCLUDED_PRINTER_H



