// player.h

#ifndef INCLUDED_PLAYER_H
#define INCLUDED_PLAYER_H

#include "entity.h"
#include <SDL_scancode.h>
#include <vector>

// A entity class controlled by several inputs
// Has children which are dependent on the position of the position of this class
// The static directions add readability to switches

struct input_handler;
struct sound;
struct sword;

struct player final : entity
{
	player(sprite* p_sprite_down,
		   sprite* p_sprite_right,
		   sprite* p_sprite_up,
		   sword* p_sword,
		   SDL_Scancode p_up,
		   SDL_Scancode p_down,
		   SDL_Scancode p_left,
		   SDL_Scancode p_right,
		   SDL_Scancode p_action,
		   SDL_Rect p_spawn_rect,
		   int p_max_health, sound* p_hit_sound);
	~player();

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;

	int get_direction(); // 0 = North, 1 = East, 2 = South, 3 = West 
	int get_health();
	int get_max_health();
private:
	static const int DIRECTIONS = 4; // It's a top-down, so four move directions are possible
	static const int NORTH = 0;
	static const int EAST = 1;
	static const int SOUTH = 2;
	static const int WEST = 3;

	int m_current_direction = 0;

	int m_max_health = 0;
	int m_health = 0;

	float m_next_pos_x;
	float m_next_pos_y;

	float m_view_direction_x;
	float m_view_direction_y;

	SDL_Rect m_spawn_rect;

	input_handler* m_inputs;
	
	std::vector<sprite*> m_sprite_c;

	SDL_Scancode m_buttons[DIRECTIONS];
	SDL_Scancode m_attack;
	bool m_blocked[DIRECTIONS];
	bool m_attacking;

	sound* m_hit_sound = nullptr;

	sword* m_sword;

	void tile_wall_collision(entity* p_other);
	void health_pickup_collision(entity* p_other);

	bool m_enemy_collide_current = false;
	bool m_enemy_collide_prev = false;


	// Inherited via entity

};

#endif // !INCLUDED_PLAYER_H



