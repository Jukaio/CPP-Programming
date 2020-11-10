// win_state.cpp

#include "win_state.h"

win_state::win_state(std::string p_name)
    : state(p_name)
    , m_timer_duration(1000)
    , m_timer(m_timer_duration)
{
    m_printer->load("win", 
                    "YOU WON", 
                    m_textures->get("font"),
                    m_text_maps->get("red"));
}

win_state::~win_state()
{
}

void win_state::enter()
{
    m_music->play("win");
}

bool win_state::update(int p_ms)
{
    if(m_timer >= 0.0f)
        m_timer -= p_ms;
    else
    {
        if(m_inputs->key_down(CONTROLS::START_BUTTON))
        {
            m_next_state = "menu";
            return false;
        }
        return true;
    }
}

void win_state::render()
{
    m_printer->draw("win", { CONFIG::UNSCALED_SCREEN_WIDTH / 2 - 32, CONFIG::UNSCALED_SCREEN_HEIGHT / 2, 100, 100 });
}

void win_state::exit()
{
    m_music->stop();
}
