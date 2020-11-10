// entity.h

#ifndef INCLUDED_ENTITY_H
#define INCLUDED_ENTITY_H

#include <SDL_rect.h>

// Base Class for all entities in the game

// Entity types, used for specific interactions inbetween game objects
enum ENTITY_TYPE
{
	UNKNOWN = -1,
	LINK,
	SWORD,
	TILE_WALL,
	ENEMY,
	HEALTH_PICK_UP
};


// forward declaration
struct sprite;
struct collider;

// Entity is updatable, renderable, activatablen comparable, collidable and transformable
struct entity
{
	entity() = default;
	virtual ~entity()
	{
		delete m_collider;
	};

	// Comparable: You can compare entities to each other based on the type
	ENTITY_TYPE get_type() const { return m_type; };

	// Transformable: Each entity has a position in x and y
	int get_position_x() const { return m_x; };
	int get_position_y() const { return m_y; };

	// Transformable and renderable are dependent
	// destination rect could get replaced with a tranformation matrix
	SDL_Rect get_destination_rect() const
	{
		return {m_x, m_y, m_w, m_h};
	}

	// Updatable: Each entitiy can get updated  and this functionality can get activated or deactivated
	bool is_active() const { return m_active; }

	// Renderable: Each entity can get drawn
	// Visibility can get used for culling, though it is not implemented
	bool is_visible() const { return m_visible; }

	// Similar method can get implemented for render (i.e. set_visible(bool))
	void set_active(const bool p_active) 
	{ 
		if(p_active)	on_enable(); 
		else			on_disable(); 
		m_active = p_active;
	}

	// Return the collider so entities can get checked for collision 
	collider* get_collider() const { return m_collider; };

	// Pure virtual functionality
	// Entity is a pure virtual function and the virtual methods have to get implemented to get a game object running
	// It allows to have collections of game objects with different functionality 
	virtual void on_enable() = 0;
	virtual void update(int p_ms) = 0;
	virtual void render() = 0;
	virtual void on_disable() = 0;
	virtual void on_collision(entity* p_other) = 0;

	virtual void reset() = 0;

protected:
	// Transform
	int m_x = 0;
	int m_y = 0;
	int m_w = 0;
	int m_h = 0;
	int m_scale = 0;

	bool m_active = true;			// If inactive, do not update
	bool m_visible = true;			// If inactive, do not render
	sprite* m_sprite = nullptr;
	collider* m_collider = nullptr;
	ENTITY_TYPE m_type = UNKNOWN;
};

#endif // !INCLUDED_ENTITY_H