// weird_forest.cpp

#include "weird_forest.h"
#include "template_test.h"

#include <audio/sound_handler.h>
#include <audio/music_player.h>



namespace CONFIG
{
    const char TITLE[] = "Weird Forest";
    const int WINDOW_WIDTH = 488;
    const int WINDOW_HEIGHT = 488;
    const float WINDOW_SCALE = 1.0f;
}

application* application::implement()
{
	return new weird_forest(CONFIG::WINDOW_WIDTH, CONFIG::WINDOW_HEIGHT, CONFIG::WINDOW_SCALE, CONFIG::TITLE);
}

weird_forest::weird_forest(int p_window_w, int p_window_h, float p_scale, const char* p_title)
	: application(p_window_w, p_window_h, p_scale, p_title)
{
}

void weird_forest::application_enter()
{
    std::string path =  "assets/texture.png";
    rectangle rect = { 20, 20, 20, 20 };
    sprite* to_return = nullptr;
    if(texture_cache->load(m_renderer, path))
        to_return = create_sprite(texture_cache->get(path), rect).m_sprite;

    auto test = service<sound_handler>::get()->create_sound("assets/sound.mp3");
    test.m_sound->play(0, 5000);
    
    
    
 
    
}

void weird_forest::application_exit()
{
}
