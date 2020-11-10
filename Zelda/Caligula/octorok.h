// enemy.h

#ifndef INCLUDED_ENEMY_H
#define INCLUDED_ENEMY_H

#include "entity.h"
#include <vector>

// Enemy class, similar to the player class, but instead of input handling (buttons, keycodes),
// it gets controlled through timers 

struct projectile;
struct sound;

struct octorok : entity
{
	octorok();
	octorok(sprite* p_sprite,
		  SDL_Rect p_spawn_rect,
		  int p_max_health, sound* p_death_sound,
		  std::vector<projectile*>* p_bullets, int p_attack_cooldown);
	~octorok();

	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;

private:
	int m_max_hp;
	int m_hp;
	bool m_collide_current = false;
	bool m_collide_prev = false;

	std::vector<projectile*>* m_bullets = nullptr;
	sound* m_death = nullptr;
	int index = 0;
	int m_attack_timer = 0;
	int m_attack_cooldown = 0;
};

#endif // !INCLUDED_ENEMY_H



