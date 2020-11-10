// GUI.h

#ifndef INCLUDED_GUI_H
#define INCLUDED_GUI_H

#include "entity.h"
#include <string>
#include <vector>

// Entity derived class with a extra sprites and text to draw a HUD
// Rendering uses the same draw function as other entities, but it passes
// a bool to tell the draw function that it is position is not relative to the the camera
// it's position is always fix

struct player;
struct sprite;

struct GUI : entity
{
	GUI(std::string p_name);

	void set_text(std::string p_text, SDL_Rect p_bounds);
	void set_hearts(sprite* p_heart, player* p_link, int p_x, int p_y);

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;
	
private:
	std::string m_name;
	int m_text_x = 0;
	int m_text_y = 0;
	int m_text_w = 0;
	int m_text_h = 0;

	sprite* m_heart_sprite;
	player* m_link;
	int m_heart_x = 0;
	int m_heart_y = 0;
};

#endif // !INCLUDED_GUI_H



