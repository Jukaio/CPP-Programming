// sound_handler.cpp

#include <SDL_mixer.h>

#include "audio/sound_handler.h"

sound_handler::sound_handler()
    : m_size(0)
{
    Mix_ChannelFinished(clean_channel);
}

sound_handler::~sound_handler()
{
}

sound_handler::sound_info sound_handler::create_sound(std::string path)
{
    sound* temp = nullptr;
    int id = -1;
    Mix_Chunk* chunk = Mix_LoadWAV("assets/sound.mp3");
    if(chunk)
    { 
        temp = new sound(chunk);
        id = m_size;
        m_cache.add(id, temp);
        m_size++;
    }
    return {id, temp};
}

void sound_handler::clean_channel(int channel)
{
    Mix_ExpireChannel(channel, 0);
}