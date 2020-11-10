// selection.h
#ifndef INCLUDED_SELECTION_H
#define INCLUDED_SELECTION_H

#include "multiple_images.h"

// A derived class of multiple images
// Adds a "selection" component to the multiple images
// Used for lists

struct selection : multiple_images
{
	selection(bool p_selectable, static_image p_hover);

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;

	bool is_selectable();
	void set_selectable(bool p_selectable);
	bool is_selected();
	void set_selected(bool p_selected);

private:
	bool m_selectable;
	bool m_selected;
	static_image m_hover_image;
};

#endif / !INCLUDED_SELECTION_H
