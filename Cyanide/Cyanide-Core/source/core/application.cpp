// application.cpp

#include "core/application.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// Application Base Class
application::application(int p_window_w, int p_window_h, float p_scale, const char* p_title)
{
    init_SDL(p_window_w, p_window_h, p_scale, p_title);
    init_managers();
}

void application::init_SDL(int p_window_w, int p_window_h, float p_scale, const char* p_title)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());

    // Initialise SDL_Mixer (Sound and Music)
    if(Mix_Init(MIX_INIT_MP3) == 0)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL_mixer: %s", Mix_GetError());
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize OpenAudio: %s", Mix_GetError());

    // Initialise SDL_Image (Enabled the loading of PNG and JPG)
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize IMG: %s", IMG_GetError());

    if(SDL_CreateWindowAndRenderer(p_window_w, p_window_h, 0, &m_window, &m_renderer) == -1)
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create SDL_Window and/or SDL_Renderer: %s", SDL_GetError());

    SDL_SetWindowTitle(m_window, p_title);

    SDL_RenderSetScale(m_renderer, p_scale, p_scale);
}

void application::init_managers()
{
    m_state_machine.set_renderer(m_renderer);
    m_input_handler.set_window(m_window);

    service<input_handler>::set(&m_input_handler);
    service<texture_manager>::set(&m_texture_manager);
    service<sprite_handler>::set(&m_sprite_handler);
    service<music_player>::set(&m_music_player);
    service<sound_handler>::set(&m_sound_handler);
}

application::~application()
{
    if(m_renderer != nullptr)
        SDL_DestroyRenderer(m_renderer);

    if(m_window != nullptr)
        SDL_DestroyWindow(m_window);

    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void application::add_state(state* p_state)
{
    m_state_machine.add_state(p_state);
}

void application::remove_state(int p_id)
{
    m_state_machine.remove_state(p_id);
}

void application::set_state(int p_id)
{
    m_state_machine.set_state(p_id);
}


void application::enter()
{
    application_enter();
}

void application::run()
{
    auto tp = SDL_GetTicks();
    int dt = SDL_GetTicks() - tp;

    const int FPS = 60;
    const int FTPS = (1.0f / FPS) * 1000;

    bool running = true;
    while(running)
    {
        tp = SDL_GetTicks();

        // This stuff has to get done in the states not in the core-loop
        running = !m_input_handler.handle_events() != !m_input_handler.is_down(keyboard::key::ESCAPE);
        
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
        SDL_RenderClear(m_renderer);

        m_state_machine.run(dt);

        SDL_RenderPresent(m_renderer);

        dt = SDL_GetTicks() - tp;
        int delay_time = FTPS - dt;
        delay_time = (delay_time < 0) ? 0 : delay_time;
        SDL_Delay(delay_time);

        dt += delay_time;
    }
    m_state_machine.shut_down();
}

void application::exit()
{
    application_exit();
}
