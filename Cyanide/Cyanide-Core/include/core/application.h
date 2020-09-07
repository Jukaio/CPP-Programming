// application.h

#ifndef INCLUDED_APPLICATION_H
#define INCLUDED_APPLICATION_H

#include <vector>
#include "FSM.h"
#include "input/input_handler.h"
#include "utilities/service.h"

#include "graphics/texture_manager.h"
#include "graphics/sprite_handler.h"

#include "audio/music_player.h"
#include "audio/sound_handler.h"

// Forward Declarations
// SDL base
struct SDL_Window;
struct SDL_Renderer;
// !Forwad Declarations

class application
{
    // TODO: Decouple
    FSM m_state_machine;
    input_handler m_input_handler;
    texture_manager m_texture_manager;
    sprite_handler m_sprite_handler;
    sound_handler m_sound_handler;
    music_player m_music_player;

    void init_SDL(int p_window_w, int p_window_h, float p_scale, const char* p_title);
    void init_managers();

protected:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    void add_state(state* p_state);
    void remove_state(int p_id); 
    void set_state(int p_id);

public:
    // Static class as "factory" 
    static application* implement();

    application(int p_window_w, int p_window_h, float p_scale, const char* p_title);
    ~application();

    void enter();
    void run();
    void exit();

    virtual void application_enter() = 0;
    virtual void application_exit() = 0;
};

#endif // !INCLUDED_APPLICATION_H




