// enemy.h

#include "octorok.h"
#include "sprite_drawer.h"
#include "collider.h"
#include "service.h"
#include "texture_manager.h"
#include "projectile.h"
#include "sound.h"

octorok::octorok()
{

}

octorok::octorok(sprite* p_sprite, SDL_Rect p_spawn_rect, int p_max_health, sound* p_death_sound, std::vector<projectile*>* p_bullets, int p_attack_cooldown)
	: m_hp(p_max_health)
	, m_max_hp(p_max_health)
	, m_bullets(p_bullets)
	, m_death(p_death_sound)
	, m_attack_cooldown(p_attack_cooldown)
{
	m_sprite = p_sprite;

	m_x = p_spawn_rect.x;
	m_y = p_spawn_rect.y;
	m_w = p_spawn_rect.w;
	m_h = p_spawn_rect.h;

	m_collider = new rectangle_collider(m_x, m_y, m_w, m_h);

	m_type = ENTITY_TYPE::ENEMY;
}

octorok::~octorok()
{
}

void octorok::on_enable()
{
}

void octorok::update(int p_ms)
{
	m_collide_prev = m_collide_current;
	m_collide_current = false;

	if(m_attack_timer <= 0)
	{
		for(auto&& bullet : *m_bullets)
		{
			if(!bullet->is_active())
			{
				int dir_x = 0;
				int dir_y = 0;
				switch(index)
				{
					case 1:
						dir_x = 1;
						break;
					case 2:
						dir_y = 1;
						break;
					case 3:
						dir_x = -1;
						break;
					default:
						index = 0;
						dir_y = -1;
						break;
				}
				index++;
				bullet->fire(m_x, m_y, dir_x, dir_y, 1);
				m_attack_timer = m_attack_cooldown;
				break;
			}
		}
	}
	else
		m_attack_timer -= p_ms;
}

void octorok::render()
{
	sprite_drawer::draw(m_sprite, get_destination_rect());
	SDL_SetRenderDrawColor(service<texture_manager>::get()->m_renderer, 0, 255, 0, 255);
}

void octorok::on_disable()
{
}

void octorok::on_collision(entity* p_other)
{
	if(!m_collide_current && !m_collide_prev && p_other->get_type() == SWORD)
	{
		m_hp--;
		if(m_hp <= 0)
		{
			m_death->play();
			set_active(false);
		}
	}
	m_collide_current = true;
}

void octorok::reset()
{
	set_active(true);
	m_hp = m_max_hp;
}
