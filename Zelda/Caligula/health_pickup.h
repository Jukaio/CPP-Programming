// health_pickup.h

#ifndef INCLUDED_HEALTH_PICKUP_H
#define INCLUDED_HEALTH_PICKUP_H

#include "entity.h"

// Just an entity which disappears when it collides with another entity
// In this case: Link (Player)

struct sound;

struct health_pickup : entity
{
	health_pickup();
	health_pickup(sprite* m_sprite, SDL_Rect p_destination, sound* p_sound);
	~health_pickup();

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;

	sound* m_pick_up_sound;

};

#endif // !INCLUDED_HEALTH_PICKUP_H



