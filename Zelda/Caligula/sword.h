// sword.h

#ifndef INCLUDED_SWORD_H
#define INCLUDED_SWORD_H

#include "entity.h"
#include <SDL_image.h>

struct player;
struct sound;

struct sword : entity
{
	sword(sprite* p_north, 
		  sprite* p_east, 
		  sound* p_attack_sound,
		  float p_duration);
	~sword();

	void set_player(player* p_link);
	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;

private:
	player* m_link;
	sprite* m_north_sprite;
	sprite* m_east_sprite;
	sound* m_attack_sound;
	SDL_RendererFlip m_flip;

	int m_timer = 0.0f;
	int m_duration = 0.0f;

	void update_position();

	// Inherited via entity
	virtual void reset() override;
};

#endif // !INCLUDED_SWORD_H



