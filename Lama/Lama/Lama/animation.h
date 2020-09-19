// animation.h

#ifndef INCLUDED_ANIMATION_H
#define INCLUDED_ANIMATION_H

#include <SDL_rect.h>

struct SDL_Texture;

struct frame;
typedef struct frame frame;

typedef enum animation_type
{
    LOOP,
    BOUNCE,
    ONCE
} animation_type;

typedef struct frame_data
{
    SDL_Rect rect;
    int frame_time;
} frame_data;

struct frame
{
    frame_data data;
    frame* prev;
    frame* next;
}; // 28 bytes 

typedef struct animation
{
    SDL_Texture* texture;
    frame* first;
    frame* current;
    frame* frames;
    int timer;
} animation; // 28 bytes


animation animation_create(SDL_Texture* p_texture, 
                           frame_data data[], 
                           int p_frame_count, 
                           animation_type p_type);
void animation_animate(animation* p_anim);
void animation_step_forward(animation* p_anim);
void animation_step_backwards(animation* p_anim);
void animation_draw(const SDL_Renderer* p_renderer, 
                    animation* p_anim, 
                    SDL_Rect* dest);
void animation_free(animation* p_anim);
void animation_reset(animation* p_anim);

#endif // !INCLUDED_ANIMATION_H
