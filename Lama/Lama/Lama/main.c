// Lama.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <string.h>
#include <time.h>

#include "grid.h"
#include "input.h"
#include "system_info.h"
#include "grass.h"

int error_msg(const char* msg)
{
    printf("Error: %s \n", msg);
    return -1;
}

typedef struct sheep
{
    int age;
    int timer;
    int hunger;
    int health;

    SDL_Rect source;
    SDL_Rect destination;
} sheep;

//´ANIMATION
struct frame
{
    SDL_Rect rect;
    struct frame* next;
    int frame_time;
}; // 28 bytes 

typedef struct animation
{
    SDL_Texture* texture;
    struct frame* current;
    struct frame* frames;
    int timer;
} animation; // 28 bytes

typedef enum animation_type
{
    LOOP
} animation_type;

animation animation_create(SDL_Texture* p_texture, SDL_Rect p_rects[], int p_frame_count, int p_frame_time, animation_type p_type)
{
    animation       anim; 
    struct frame*   frames;
    int             index;

    frames          = (struct frame*)calloc(p_frame_count, sizeof(struct frame));
    anim.current    = frames;
    anim.frames     = frames;
    anim.texture    = p_texture;
    anim.timer      = p_frame_time;

    switch(p_type)
    {
        case LOOP:
        // Create the animation in here
            for(index = 0; index < p_frame_count - 1; index++) //loop through all frames until the last one
            {
                frames[index].rect          = p_rects[index];
                frames[index].frame_time    = p_frame_time;
                frames[index].next          = &frames[index + 1];
            }
            frames[index].rect          = p_rects[index];
            frames[index].frame_time    = p_frame_time;
            frames[index].next          = frames;
        break;
    }
    return anim;
}

void animation_animate(animation* p_anim)
{
    p_anim->timer--;
    if(p_anim->timer < 0)
    {
        p_anim->current = p_anim->current->next;
        p_anim->timer = p_anim->current->frame_time;
    }
}

void animation_step_forward(animation* p_anim)
{
    p_anim->current = p_anim->current->next;
    p_anim->timer = p_anim->current->frame_time;
}

void animation_draw(const SDL_Renderer* p_renderer, animation* p_anim, SDL_Rect* dest)
{
    SDL_RenderCopy(p_renderer, p_anim->texture, &p_anim->current->rect, dest);

    SDL_SetRenderDrawColor(p_renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(p_renderer, &p_anim->current->rect);

    animation_animate(p_anim);
}

void animation_free_frames(animation* p_anim)
{
    if(p_anim->frames)
        free(p_anim->frames);
}
// !ANIMATION 

typedef struct sheep_flock
{
    int source_index_step; // grass' index step (animation)
    int flock_size;
    grid* grid_layer;
    sheep* pool;
} sheep_flock;



typedef struct wolf
{
    int age;
    int timer;

    SDL_Rect source;
    SDL_Rect destination;
} wolf;

typedef struct wolf_pack
{
    int source_index_step; // grass' index step (animation)
    int flock_size;
    grid* grid_layer;
    wolf* pool;
} wolf_pack;


typedef struct application
{
    SDL_Renderer* renderer;
    window_info* window;
    grid* grid_layer;
    struct mouse* mouse_input;
} application;


int main(int argc, char* argv[])
{
    mouse           mouse_input;
    window_info     window_settings;
    SDL_Event       event;
    vector2         mouse_position_on_grid;
    application     app;
    grid            grid_layer;
    SDL_Color       background_color;
    Uint32          alpha_color_key;
    SDL_Rect        grass_source_rect;
    grass_field     grass_pool;
    time_t          rawtime;
    struct tm*      time_info               = NULL;
    SDL_Texture*    texture                 = NULL;
    SDL_Surface*    surface                 = NULL;
    SDL_Window*     window                  = NULL;
    SDL_Renderer*   renderer                = NULL;
    bool            running                 = true;
    int             animation_step          = 32;

    // Set seed based on time
    time(&rawtime);
    time_info = (struct tm*) malloc(sizeof(struct tm));
    localtime_s(time_info , &rawtime);
    srand(time_info->tm_year + time_info->tm_mon + time_info->tm_min + time_info->tm_sec);
    
    struct frame frames[5] = { {.rect       = {0, 0, 0, 0}, 
                                .next       = &frames[1],
                                .frame_time = 200},
                             };

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        return error_msg("Could not Initialise SDL");

    memset(&mouse_input, NULL, sizeof(mouse_input));

    window_settings.title = "Lama";
    window_settings.dimension.x = 640;
    window_settings.dimension.y = 640;
    window_settings.position.x = SDL_WINDOWPOS_CENTERED;
    window_settings.position.y = SDL_WINDOWPOS_CENTERED;
    

    if(!grid_init(&grid_layer, window_settings.dimension, 64, 0xFF0000FF))
        return error_msg("Could not initialise grid");

    window = SDL_CreateWindow(window_settings.title,
                              window_settings.position.x,
                              window_settings.position.y,
                              window_settings.dimension.x,
                              window_settings.dimension.y,
                              NULL);

    renderer = SDL_CreateRenderer(window,
                                  NULL, 
                                  NULL);


    surface = SDL_LoadBMP("texture.bmp");

    alpha_color_key = (244 << 16) + (93 << 8) + (146 << 0);
    if(SDL_SetColorKey(surface, SDL_TRUE, alpha_color_key) != 0)
        return error_msg("Surface not valid");

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    background_color.r = 214;
    background_color.g = 185;
    background_color.b = 123;
    background_color.a = 255;

    app.grid_layer = &grid_layer;
    app.mouse_input = &mouse_input;
    app.renderer = renderer;
    app.window = window;

    grass_source_rect.x = 0;
    grass_source_rect.y = 0;
    grass_source_rect.w = 32;
    grass_source_rect.h = 32;

    grass_field_init(&grass_pool,
                           &grid_layer,
                           grid_layer.dimensions,
                           grass_source_rect,
                           500,
                           2000,
                           animation_step,
                           grid_layer.cell_size);

    vector2 grass_index = {4, 6};

    SDL_Rect rects[5] = { { 0, 0, 32, 32},
                          { 32, 0, 32, 32},
                          { 64, 0, 32, 32},
                          { 96, 0, 32, 32},
                          { 128, 0, 32, 32} };

    SDL_Rect destination = { 0, 0, 128, 128 };

    animation anim = animation_create(texture, rects, 5, 200, LOOP);

    while(running)
    {
        // Input Events
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                mouse_input.buttons = SDL_GetMouseState(&mouse_input.position.x, 
                                                        &mouse_input.position.y);
                break;
            }
        }
        if(mouse_input.buttons & LEFT_CLICK)
        {
            mouse_position_on_grid.x = (int)(mouse_input.position.x / grid_layer.cell_size);
            mouse_position_on_grid.y = (int)(mouse_input.position.y / grid_layer.cell_size);

            grass_field_toggle_active_at(&grass_pool, mouse_position_on_grid, true);
            //set_grid_data_at(&grid_layer, 
            //                 mouse_position_on_grid, 
            //                 GRASS, 
            //                 true);
        }
        else if(mouse_input.buttons & RIGHT_CLICK)
        {
            mouse_position_on_grid.x = (int)(mouse_input.position.x / grid_layer.cell_size);
            mouse_position_on_grid.y = (int)(mouse_input.position.y / grid_layer.cell_size);
            grass_field_toggle_active_at(&grass_pool, mouse_position_on_grid, false);
            //set_grid_data_at(&grid_layer, 
            //                 mouse_position_on_grid, 
            //                 GRASS, 
            //                 false);
        }

        // !Input Events

        // Update
        grass_field_update(&grass_pool,
                           &grid_layer);
        // !Update

        // Render
        SDL_SetRenderDrawColor(renderer,
                               background_color.r,
                               background_color.g,
                               background_color.b,
                               background_color.a);
        SDL_RenderClear(renderer);

        grass_field_draw(renderer,
                        texture,
                        &grass_pool,
                        grid_layer.dimensions);

        grid_draw(renderer, &grid_layer);

        animation_animate(&anim);
        animation_draw(renderer, &anim, &destination);

        SDL_RenderPresent(renderer);
        // !Render
    }

    grass_field_clean(&grass_pool);
    grid_free_nodes(&grid_layer);

    SDL_Quit();

    return 0;
}