// player.cpp

#include "player.h"
#include "sprite_drawer.h"
#include "input_handler.h"
#include "service.h"
#include <math.h>
#include "collider.h"
#include "sound.h"
#include "sword.h"
#include "camera.h"
#include "config.h"

// A lot of the math-heavy things in here could have been replaced with a vector2 math library
// I did not want to focus on such a library, thus I rather worked with vanilla ints and floats
// I focused on resource management and such 

player::player(sprite* p_sprite_down,
			   sprite* p_sprite_right,
			   sprite* p_sprite_up,
			   sword* p_sword,
			   SDL_Scancode p_up,
			   SDL_Scancode p_down,
			   SDL_Scancode p_left,
			   SDL_Scancode p_right,
			   SDL_Scancode p_action,
			   SDL_Rect p_spawn_rect,
			   int p_max_health, 
			   sound* p_hit_sound)
	: m_spawn_rect(p_spawn_rect)
	, m_next_pos_x(p_spawn_rect.x)
	, m_next_pos_y(p_spawn_rect.y)
	, m_blocked{false}
	, m_attacking(false)
	, m_sword(p_sword)
	, m_health(p_max_health)
	, m_max_health(p_max_health)
	, m_hit_sound(p_hit_sound)

{
	m_type = LINK;
	m_inputs = service<input_handler>::get();
	
	m_buttons[NORTH] = p_up;
	m_buttons[EAST] = p_right;
	m_buttons[SOUTH] = p_down;
	m_buttons[WEST] = p_left;
	m_attack = p_action;

	m_sprite_c.push_back(p_sprite_up);
	m_sprite_c.push_back(p_sprite_right);
	m_sprite_c.push_back(p_sprite_down);

	m_sprite = p_sprite_down;

	m_x = p_spawn_rect.x;
	m_y = p_spawn_rect.y;
	m_w = p_spawn_rect.w;
	m_h = p_spawn_rect.h;
	m_collider = new rectangle_collider(m_x, m_y, m_w, m_h);
}

player::~player()
{
	if(m_collider != nullptr)
		delete m_collider;
	m_collider = nullptr;
}

void player::on_enable()
{
}

void player::update(int p_ms)
{
	// Start with a null vector
	float velocity_x = 0; 
	float velocity_y = 0;

	// Add the pressed directions to the vector (If not blocked)
	// x_axis
	if(!m_sword->is_active())
	{ 
		if(m_inputs->key_down(m_buttons[EAST]) && !m_blocked[EAST])
		{ 
			m_sprite = m_sprite_c[EAST];
			m_current_direction = EAST;
			velocity_x += 1.0f;
		}
		if(m_inputs->key_down(m_buttons[WEST]) && !m_blocked[WEST])
		{ 
			m_sprite = m_sprite_c[EAST];
			m_current_direction =WEST;
			velocity_x += -1.0f;
		}
		// y-axis
		if(m_inputs->key_down(m_buttons[NORTH]) && !m_blocked[NORTH])
		{ 
			m_sprite = m_sprite_c[NORTH];
			m_current_direction = NORTH;
			velocity_y += -1.0f;
		}
		if(m_inputs->key_down(m_buttons[SOUTH]) && !m_blocked[SOUTH])
		{ 
			m_sprite = m_sprite_c[SOUTH];
			m_current_direction = SOUTH;
			velocity_y += 1.0f;
		}

		// Attack
		if(m_inputs->key_pressed(m_attack))
		{
			m_sword->set_active(true);
		}
	}

	// Get the unit vector of the direction (8-Axis movement)
	float magnitude = std::sqrtf((velocity_x * velocity_x) + (velocity_y * velocity_y));
	if(magnitude > 0)
	{
		velocity_x /= magnitude;
		velocity_y /= magnitude;
	}

	// Have a floating point instead of an int to avoid jittering (otherwhise the casts ruin the movement or 
	// you cannot even move since the values may be too small
	m_next_pos_x += velocity_x;
	m_next_pos_y += velocity_y;

	// Cast the float position to the int position so it gets drawn in the right spot
	m_x = (int) std::roundf(m_next_pos_x);
	m_y = (int) std::roundf(m_next_pos_y);

	// Set camera based on player position
	//camera::instance()->set_position(m_x - (CONFIG::UNSCALED_SCREEN_WIDTH / 2),
	//								 m_y - (CONFIG::UNSCALED_SCREEN_HEIGHT / 2));

	// Update collider position so collison works
	m_collider->set_position(m_x, m_y);

	// Reset the blocks for the next frame (Input Handling -> Collision Handling -> Update -> Render) 
	for (auto&& i : m_blocked)
		i = false;

	// only set view direction if the player is actually moving
	if(!(velocity_x == 0.0f &&
		 velocity_y == 0.0f))
	{
		m_view_direction_x = velocity_x;
		m_view_direction_y = velocity_y;
	}

	// keep track of collisions
	m_enemy_collide_prev = m_enemy_collide_current;
	m_enemy_collide_current = false;

}

void player::render()
{
	if(m_view_direction_x < 0 && m_sprite == m_sprite_c[EAST])
		sprite_drawer::draw(m_sprite, get_destination_rect(), SDL_FLIP_HORIZONTAL);
	else
		sprite_drawer::draw(m_sprite, get_destination_rect());
}

void player::on_disable()
{
}

void player::on_collision(entity* p_other)
{
	switch(p_other->get_type())
	{
		case TILE_WALL:
			tile_wall_collision(p_other);
			break;

		case HEALTH_PICK_UP:
			health_pickup_collision(p_other);
			break;

		case ENEMY:
			// Get damage when anything like an enemy hits you
			// Flicker animation, back bounce, etc can get implemented here
			if(!m_enemy_collide_current && !m_enemy_collide_prev)
			{
				m_hit_sound->play();
				m_health--;
				if(m_health <= 0)
					set_active(false);
			}
			m_enemy_collide_current = true;
		default:
			break;
	}
}

int player::get_direction()
{
	return m_current_direction;
}

int player::get_health()
{
	return m_health;
}

int player::get_max_health()
{
	return m_max_health;
}

void player::tile_wall_collision(entity* p_other)
{
	float direction_x = p_other->get_position_x() - m_x;
	float direction_y = p_other->get_position_y() - m_y;

	float bound_factor = 1.0f / std::sqrtf((1 * 1) + (1 * 1)); // This vector gives a bound value so I have a rather perpendicular direction
															   // a single-directional vector (Such as north, south, etc) has a vector length of 1
															   // a dual-directional vector is usually bigger than 1, and if normalised we only wanna regard 
															   // what is on the "bigger" side, aka. a normal direction. In 3D I would have used the cross product
															   // That works just fine

	// Getting a unit vector so we have the direction of p_other
	float magnitude = std::sqrtf((direction_x * direction_x) + (direction_y * direction_y));
	float unit_x, unit_y;
	if(magnitude > 0)
	{
		direction_x /= magnitude;
		direction_y /= magnitude;
	}

	// Checking what the normal direction is (North, South, West, East)
	// With 2D vectors I would have only made a static const vector2
	// Could have saved time and energy
	// but that works too and refreshes my math data base a little
	if(std::abs(direction_x) > bound_factor)
	{
		// Check whether the direction on x is going east or west
		// Based on that we block the direction so the input becomes "meaningless"
		if(direction_x > 0)
			m_blocked[EAST] = true;
		else if(direction_x < 0)
			m_blocked[WEST] = true;
	}

	if(std::abs(direction_y) > bound_factor)
	{
		// What I described/explained for the x-axis, but for the y-axis
		if(direction_y > 0)
			m_blocked[SOUTH] = true;
		else if(direction_y < 0)
			m_blocked[NORTH] = true;
	}
}

void player::health_pickup_collision(entity* p_other)
{
	if(m_health < m_max_health)
		m_health++;
}

void player::reset()
{
	set_active(true);
	m_health = m_max_health;
	m_next_pos_x = m_spawn_rect.x;
	m_next_pos_y = m_spawn_rect.y;
}
