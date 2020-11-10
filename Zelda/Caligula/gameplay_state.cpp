// gameplay_state.cpp  

#include "gameplay_state.h"
#include "pugixml.hpp"
#include "tile_set.h"
#include "static_image.h"
#include "layer.h"
#include "config.h"
#include "player.h"
#include "health_pickup.h"
#include "sword.h"
#include "octorok.h"
#include "projectile.h"
#include "GUI.h"
#include "camera.h"

using namespace GAMEPLAY_SCREEN;

gameplay_state::gameplay_state(std::string p_name)
    : state(p_name)
{  
    // Create tile set
    m_tile_set = new tile_set("underworld_blue", 
                              m_textures->get("underworld_blue"), 
                              UNDERWORLD_BLUE_ROWS, 
                              UNDERWORLD_BLUE_COLUMNS, 
                              CONFIG::TILE_SIZE);

    // load map data from xml file

    for(int i = 0; i < AMOUNT_OF_LEVELS; i++)
    {
        std::string base = "../Assets/gameplay_state/level_";
        std::string end = ".xml";

        std::string path = base + std::to_string(i) + end;

        pugi::xml_document document;
        pugi::xml_parse_result result = document.load_file(path.c_str());

        auto map_string = document.child("map").child("layer").child("data").child_value();
        int map_width = document.child("map").attribute("width").as_int();
        int map_height = document.child("map").attribute("height").as_int();
        int map_tile_width = document.child("map").attribute("tilewidth").as_int();
        int map_tile_height = document.child("map").attribute("tileheight").as_int();
        int first_id = document.child("map").child("tileset").attribute("firstgid").as_int();
    
        std::vector<int> tile_ids;
        const char* c = map_string;
        std::string temp_string = "";
	
        for(const char* c = map_string; *c != '\0'; c++)
        {
            if(*c != ',' && *c != '\n')
                temp_string += *c;
            else
            {
                if(!temp_string.empty())
                    tile_ids.push_back(std::stoi(temp_string));
                temp_string = "";
            }
        }
    
        // Create layer
        m_layers.push_back(new layer(m_tile_set,
                           map_width,
                           map_height,
                           map_tile_width,
                           map_tile_height,
                           tile_ids,
                           first_id, 
                           i * CONFIG::UNSCALED_SCREEN_WIDTH, 
                           0));
    }

    // Create sprites
    // Link
    sprite* link_down_sprite = m_sprites->load_and_get("gameplay_link_down", m_textures->get("link"), LINK_DOWN_SRC_RCT);
    sprite* link_right_sprite = m_sprites->load_and_get("gameplay_right_down", m_textures->get("link"), LINK_RIGHT_SRC_RCT);
    sprite* link_up_sprite = m_sprites->load_and_get("gameplay_up_down", m_textures->get("link"), LINK_UP_SRC_RCT);

    // sword
    sprite* sword_up = m_sprites->load_and_get("sword_up", m_textures->get("link"), SWORD_UP_SRC);
    sprite* sword_right = m_sprites->load_and_get("sword_right", m_textures->get("link"), SWORD_RIGHT_SRC);

    // enemies
    sprite* octorok_sprite = m_sprites->load_and_get("octorok", m_textures->get("enemies"), OCTOROK_SRC_RCT);

    // bullet
    sprite* bullet = m_sprites->load_and_get("bullet", m_textures->get("enemies"), OCTOROK_BULLET_SRC_RCT);

    // HP Pick Up
    sprite* h_up = m_sprites->load_and_get("health_pick_up", m_textures->get("link"), HP_PICK_UP_SRC_RCT);

    // Load sounds
    // Link
    m_sounds->load("link_attack", "../Assets/gameplay_state/LOZ_Sword_Slash.wav");

    // HP Pick Up
    m_sounds->load("pick_up", "../Assets/gameplay_state/LOZ_Get_Item.wav");

    // Enemy projectile hit
    m_sounds->load("enemy_hit", "../Assets/gameplay_state/LOZ_Enemy_Hit.wav");

    // Enemy Death
    m_sounds->load("enemy_death", "../Assets/gameplay_state/LOZ_Enemy_Die.wav");

    // Get Sounds
    // Link
    sound* sound_attack = m_sounds->get("link_attack");

    // HP Pick Up
    sound* pick_up_sound = m_sounds->get("pick_up");
    
    // Enemy projectile hit
    sound* enemy_hit = m_sounds->get("enemy_hit");

    // Enemy Death
    sound* enemy_death = m_sounds->get("enemy_death");

    // Create Link/Player
    m_sword = new sword(sword_up, sword_right, sound_attack, LINK_ATTACK_STAGGER);
    m_link = new player(link_down_sprite,
                        link_right_sprite,
                        link_up_sprite,
                        m_sword,
                        CONTROLS::UP_BOTTON,
                        CONTROLS::DOWN_BOTTON,
                        CONTROLS::LEFT_BOTTON,
                        CONTROLS::RIGHT_BOTTON,
                        CONTROLS::START_BUTTON,
                        LINK_SPAWN_DST_RCT,
                        LINK_MAX_HP, enemy_hit);
    m_sword->set_player(m_link);

    // Put Layers in object list
    for(int index = 0; index < AMOUNT_OF_LEVELS; index++)
        m_entities.push_back(m_layers[index]);
    

    // Put Link in object list
    m_entities.push_back(m_sword); // Layer order = 1
    m_entities.push_back(m_link); // Layer order = 2


    // Create health pick ups and put it in object list
    m_health_pickups.reserve(HP_PICK_UP_AMOUNT);
    for(int i = 0; i < HP_PICK_UP_AMOUNT; i++)
    {
        m_health_pickups.push_back(new health_pickup(h_up, HP_PICK_UP_SPAWNS[i], pick_up_sound));
        m_entities.push_back(m_health_pickups[i]); // Layer order = 3
    }

    m_bullets.reserve(10); // create bullets
    for(int i = 0; i < 10; i++)
    {
        m_bullets.push_back(new projectile(bullet, { 20, 40, 8, 16}));
        m_entities.push_back(m_bullets[i]);
    }

    m_enemies.reserve(ENEMY_AMOUNT); // for enemies
    for(int i = 0; i < ENEMY_AMOUNT; i++)
    {
        int random_attack_speed = 1000 + ((2 - (rand() & 4)) * 100);
        m_enemies.push_back(new octorok(octorok_sprite, ENEMY_SPAWNS[i], 2, enemy_death, &m_bullets, random_attack_speed));
        m_entities.push_back(m_enemies[i]); // Layer order = 4
    }

    m_gui = new GUI("gui");
    m_gui->set_text("Enemies left - " + std::to_string(ENEMY_AMOUNT), { 8, 200, 152, 8 });
    m_gui->set_hearts(m_sprites->get("life_heart"), m_link, 160, 200);
    m_entities.push_back(m_gui);

    // Set camera
    camera::instance().set_position(0, 0);
}

gameplay_state::~gameplay_state()
{
    if(m_tile_set != nullptr)
        delete m_tile_set;
    m_tile_set = nullptr;
}

void gameplay_state::enter()
{
    m_music->play("play");
    for(auto&& e : m_entities)
    {
        e->reset();
    }
}

bool gameplay_state::update(int p_ms)
{
    for(int index = 0; index < AMOUNT_OF_LEVELS; index++)
    { 
        for(int i = 0; i < m_layers[index]->size(); i++)
        {   
            tile* other = m_layers[index]->get_tile(i);
            if(other->get_collider() == nullptr)
                continue;

            if(m_collisions->is_colliding(m_link->get_collider(), other->get_collider()))
            {
                m_link->on_collision(other);
                // The tiles do not have any behaviour - No collision call for them
            }
        }
    }
    for(int i = 0; i < HP_PICK_UP_AMOUNT; i++)
    {
        if(m_collisions->is_colliding(m_link->get_collider(), m_health_pickups[i]->get_collider()))
        {
            if(m_health_pickups[i]->is_active())
            { 
                m_link->on_collision(m_health_pickups[i]);
                m_health_pickups[i]->on_collision(m_link);
            }
        }
    }

    // Check Colllision enemy with sword
    for(int i = 0; i < ENEMY_AMOUNT; i++)
    {
        if(m_sword->is_active() && m_collisions->is_colliding(m_sword->get_collider(), m_enemies[i]->get_collider()))
        {
            if(m_enemies[i]->is_active())
            {
                m_enemies[i]->on_collision(m_sword);
            }
        }
    }

    // Check Collision with link
    for(int i = 0; i < ENEMY_AMOUNT; i++)
    {
        if(m_link->is_active() && m_collisions->is_colliding(m_link->get_collider(), m_enemies[i]->get_collider()))
        {
            if(m_enemies[i]->is_active())
            {
                m_link->on_collision(m_enemies[i]);
            }
        }
    }

    // Bullet with everything
    {
        for(auto&& bullet : m_bullets)
        {

            if(!bullet->is_active())
                continue;

            if(bullet->get_collider() != nullptr)
            {
                // Check Link
                if(m_collisions->is_colliding(bullet->get_collider(), m_link->get_collider()))
                {
                    bullet->on_collision(m_link);
                    m_link->on_collision(bullet);
                }

                // Check Walls
                for(int index = 0; index < AMOUNT_OF_LEVELS; index++)
                {
                    for(int i = 0; i < m_layers[index]->size(); i++)
                    {
                        tile* other = m_layers[index]->get_tile(i);

                        if(other->get_collider() == nullptr)
                            continue;

                        if(m_collisions->is_colliding(bullet->get_collider(), other->get_collider()))
                        {
                            bullet->on_collision(other);
                            // The tiles do not have any behaviour - No collision call for them
                        }
                    }
                }
            }
        }
    }

    // Updates all entities 
    for(auto&& e : m_entities)
    {
        if(e->is_active())
            e->update(p_ms);
    }

    // GUI and information processing
    int enemies_alive = 0;
    for(auto&& e : m_enemies)
    {
        if(e->is_active())
            enemies_alive++;
    }
    m_gui->set_text("Enemies left - " + std::to_string(enemies_alive), { 8, 200, 152, 8 });

    // Checks for lose condition
    if(!m_link->is_active())
    {
        m_next_state = "lose";
        return false;
    }

    // Checks for win condition
    if(enemies_alive <= 0)
    {
        m_next_state = "win";
        return false;
    }

    camera::instance().set_position(m_link->get_position_x() - (CONFIG::UNSCALED_SCREEN_WIDTH / 2), 
                                    m_link->get_position_y() - (CONFIG::UNSCALED_SCREEN_HEIGHT / 2));

    auto horizontal_index = m_link->get_position_x() / CONFIG::UNSCALED_SCREEN_WIDTH;
    camera::instance().set_position(((int) horizontal_index) * CONFIG::UNSCALED_SCREEN_WIDTH,
                                    0);

    return true;
}

void gameplay_state::render()
{
    for(auto&& e : m_entities)
    {
        if(e->is_visible() && e->is_active())
            e->render();
    }
}

void gameplay_state::exit()
{
    m_music->stop();
}
