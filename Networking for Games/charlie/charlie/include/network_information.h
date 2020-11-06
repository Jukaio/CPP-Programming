// network_information.h

#ifndef INCLUDED_NETWORK_INFORMATION_H
#define INCLUDED_NETWORK_INFORMATION_H

#include <string>

#include <charlie.hpp>

using namespace charlie;

namespace NetworkInformation
{
	constexpr float KIBIBYTE = 1024;
	// Type of Network Information
	enum Type
	{
		KIBIBYTES_SENT,
		KIBIBYTES_RECEIVED,
		KIBIBYTES_PER_SECOND_SENT,
		KIBIBYTES_PER_SECOND_RECEIVED,
		PACKETS_SENT,
		PACKETS_RECEIVED,
		PACKETS_PER_SECOND_SENT,
		PACKETS_PER_SECOND_RECEIVED,
		PACKET_LOSS,
		CURRENT_RTT,
		MIN_RTT,
		MAX_RTT,
		AVG_RTT,
		TOTAL_MESSAGES_SENT,
		TOTAL_MESSAGES_RECEIVED,
		INPUT_MISSPREDICTIONS,

		NETWORK_DATA_TYPE_COUNT
	};

	struct Overlay
	{
		Overlay(float (&p_listen_to)[NETWORK_DATA_TYPE_COUNT],
				std::string (&p_titles)[NETWORK_DATA_TYPE_COUNT])
			: m_listen_to(p_listen_to)
			, m_titles(p_titles)
		{

		}

		static void draw(const Overlay& p_overlay, Renderer& p_renderer, 
						 int x, 
						 int y,
						 int size,
						 Color color = Color::Red,
						 int spacing = 208,	// spacing is equal to a random value which seems fitting
						 int offset = 8)		// offset is equal to the height of one char
		{
			for(int i = 0; i < NETWORK_DATA_TYPE_COUNT; i++)
			{
				draw_text(p_renderer,
						  x,
						  y + (offset * i * size),
						  spacing * size,
						  size,
						  color,
						  p_overlay.m_titles[i].c_str(),
						  p_overlay.m_listen_to[i]);
			}
		}

	private:
		static void draw_text(Renderer& p_renderer, 
							  int x, 
							  int y, 
							  int spacing_x, 
							  int size,
							  Color color,
							  const char* title, 
							  float value)
		{
			std::string value_string = std::to_string(value);
			value_string.erase(value_string.find_last_not_of('0') + 1, std::string::npos);
			if(value_string.at(value_string.length() - 1) == '.')
				value_string.erase(value_string.begin() + value_string.length() - 1);

			p_renderer.render_text({ x,
									 y },
								   color,
								   size, title);
			p_renderer.render_text({ x + spacing_x,
									 y },
								   color,
								   size, value_string.c_str());

		}

		float(&m_listen_to)[NETWORK_DATA_TYPE_COUNT];
		std::string(&m_titles)[NETWORK_DATA_TYPE_COUNT];
	};
}

#endif // !INCLUDED_NETWORK_INFORMATION_OVERLAY_H
