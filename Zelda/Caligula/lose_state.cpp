// lose_state.cpp

#include "lose_state.h"
#include "config.h"

lose_state::lose_state(std::string p_name)
    : state(p_name)
    , m_timer_duration(1000)
    , m_timer(m_timer_duration)
{
    m_printer->load("lose",
                    "YOU LOSE",
                    m_textures->get("font"),
                    m_text_maps->get("red"));
}

lose_state::~lose_state()
{
}

void lose_state::enter()
{
    m_timer = m_timer_duration;
}

bool lose_state::update(int p_ms)
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

void lose_state::render()
{
    m_printer->draw("lose", { CONFIG::UNSCALED_SCREEN_WIDTH / 2 - 32, CONFIG::UNSCALED_SCREEN_HEIGHT / 2, 100, 100 });
}

void lose_state::exit()
{
    m_music->stop();
}
