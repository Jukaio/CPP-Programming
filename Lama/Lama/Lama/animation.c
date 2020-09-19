// animation.c

#include <SDL_render.h>
#include <stdlib.h>
#include "animation.h"
#include "system_info.h"


animation animation_create(SDL_Texture* p_texture, frame_data data[], int p_frame_count, animation_type p_type)
{
    int             index;
    int             data_index;
    int             total_count;
    animation       anim;
    frame*          frames      = NULL;

    //anim = (animation*)malloc(sizeof(animation));
    switch(p_type)
    {
        case LOOP:
            total_count = p_frame_count;
            frames = (frame*)calloc(total_count, sizeof(frame));
            // Create the animation in here
            if(frames)
            {
                for(index = 0; index < total_count - 1; index++) //loop through all frames until the last one
                {
                    frames[index].data = data[index];
                    frames[index].next = &frames[index + 1];
                    frames[index + 1].prev = &frames[index];
                }
                frames[index].data = data[index];
                frames[index].next = frames;
                frames[0].prev = &frames[index];
            }
            break;

        case BOUNCE:
            total_count = ((p_frame_count - 1) * 2);
            frames = (frame*)calloc(total_count, sizeof(frame));
            if(frames)
            {
                for(index = 0; index < total_count - 1; index++) //loop through all frames until the last one
                {
                    data_index = abs(((index > p_frame_count - 1) * (total_count)) - index);
                    frames[index].data = data[data_index]; // index + (!(index < p_frame_count - 1)) * full_count 
                    frames[index].next = &frames[index + 1];
                    frames[index + 1].prev = &frames[index];
                }
                data_index = abs(((index > p_frame_count - 1) * (total_count)) - index);
                frames[index].data = data[data_index];
                frames[index].next = frames;
                frames[0].prev = &frames[index];
            }
            break;

        case ONCE:
            total_count = p_frame_count;
            frames = (frame*)calloc(total_count, sizeof(frame));
            // Create the animation in here
            if(frames)
            {
                for(index = 0; index < total_count - 1; index++) //loop through all frames until the last one
                {
                    frames[index].data = data[index];
                    frames[index].next = &frames[index + 1];
                    frames[index + 1].prev = &frames[index];
                }
                frames[index].data = data[index];
                frames[index].next = NULL;
                frames[0].prev = NULL;
            }
            break;
    }

    anim.current = frames;
    anim.frames = frames;
    anim.first = frames;
    anim.texture = p_texture;
    anim.timer = frames[0].data.frame_time;
    return anim;
}

void animation_animate(animation* p_anim)
{
    if(p_anim->current->next)
    {
        p_anim->timer -= FRAME_TIME;
        if(p_anim->timer < 0)
        {
            p_anim->current = p_anim->current->next;
            p_anim->timer = p_anim->current->data.frame_time;
        }
    }
}

void animation_step_forward(animation* p_anim)
{
    if(p_anim->current->next)
    {
        p_anim->current = p_anim->current->next;
        p_anim->timer = p_anim->current->data.frame_time;
    }
}

void animation_step_backwards(animation* p_anim)
{
    if(p_anim->current->prev)
    {
        p_anim->current = p_anim->current->prev;
        p_anim->timer = p_anim->current->data.frame_time;
    }
}

void animation_draw(const SDL_Renderer* p_renderer, animation* p_anim, SDL_Rect* dest)
{
    SDL_RenderCopy(p_renderer, p_anim->texture, &p_anim->current->data.rect, dest);

    SDL_SetRenderDrawColor(p_renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(p_renderer, &p_anim->current->data.rect);
}

void animation_reset(animation* p_anim)
{
    if(p_anim)
        p_anim->current = p_anim->first;
}

void animation_free(animation* p_anim)
{
    if(p_anim)
    {
        if(p_anim->frames)
            free(p_anim->frames);
        free(p_anim);
    }
}