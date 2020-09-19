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
#include "animation.h"
#include "sheep.h"

int error_msg(const char* msg)
{
    printf("Error: %s \n", msg);
    return -1;
}

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



int main(int argc, char* argv[])
{
    int             index;
    mouse           mouse_input;
    window_info     window_settings;
    SDL_Event       event;
    vector2         mouse_position_on_grid;
    time_t          rawtime;
    Uint32          alpha_color_key;
    SDL_Color       background_color        = { 214, 185, 123, 255};
    struct tm*      time_info               = NULL;
    SDL_Texture*    texture                 = NULL;
    SDL_Surface*    surface                 = NULL;
    SDL_Window*     window                  = NULL;
    SDL_Renderer*   renderer                = NULL;
    bool            running                 = true;

    grid            grid_layer;

    grass_field     grass_pool;
    animation       grass_animation;
    frame_data      grass_frame_info[6] = { {.rect = {0  , 0, 32, 32}, .frame_time = 2000 },
                                            {.rect = {32 , 0, 32, 32}, .frame_time = 2000 },
                                            {.rect = {64 , 0, 32, 32}, .frame_time = 2000 },
                                            {.rect = {96 , 0, 32, 32}, .frame_time = 2000 },
                                            {.rect = {128, 0, 32, 32}, .frame_time = 2000 },
                                            {.rect = {160, 0, 32, 32}, .frame_time = 2000 } };
    
    sheep_flock     sheep_pool;
    animation       sheep_animations[SHEEP_ANIMATION_COUNT];
    frame_data      sheep_frame_infos[SHEEP_ANIMATION_COUNT][3] = 
    { 
        { {.rect = {0 ,  32, 32, 32}, .frame_time = 2000 },
          {.rect = {64,  32, 32, 32}, .frame_time = 2000 },
          {.rect = {128, 32, 32, 32}, .frame_time = 2000 } },

        { {.rect = {0 ,  64, 32, 32}, .frame_time = 2000 },
          {.rect = {64,  64, 32, 32}, .frame_time = 2000 },
          {.rect = {128, 64, 32, 32}, .frame_time = 2000 } },

        { {.rect = {0 ,  96, 32, 32}, .frame_time = 2000 },
          {.rect = {64,  96, 32, 32}, .frame_time = 2000 },
          {.rect = {128, 96, 32, 32}, .frame_time = 2000 } }
    };

    // Set seed based on time
    time(&rawtime);
    time_info = (struct tm*) malloc(sizeof(struct tm));
    localtime_s(time_info , &rawtime);
    srand(time_info->tm_year + time_info->tm_mon + time_info->tm_min + time_info->tm_sec);
    
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

    int frame_count = 6;
    grass_animation = animation_create(texture, 
                                       grass_frame_info, 
                                       frame_count,
                                       ONCE);
    grass_field_init(&grass_pool,
                           &grid_layer,
                           grid_layer.dimensions,
                           grass_animation,
                           1000,
                           2000,
                           10000,
                           grid_layer.cell_size);

    frame_count = 3;
    for(index = 0; index < SHEEP_ANIMATION_COUNT; index++)
        sheep_animations[index] = animation_create(texture, 
                                                   sheep_frame_infos[index], 
                                                   frame_count, 
                                                   ONCE);
    sheep_flock_init(&sheep_pool, 
                     &grid_layer,
                     sheep_animations,
                     10,
                     20000);


    vector2 spawn_at = { 0, 0};
    sheep_flock_spawn_sheep(&sheep_pool, spawn_at);
    spawn_at.x = 8;
    sheep_flock_spawn_sheep(&sheep_pool, spawn_at);
    spawn_at.y = 5;
    sheep_flock_spawn_sheep(&sheep_pool, spawn_at);
    spawn_at.x = 2;
    sheep_flock_spawn_sheep(&sheep_pool, spawn_at);
    //animation copy = *anim;
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

            grass_field_toggle_alive_at(&grass_pool, mouse_position_on_grid, true);
        }
        else if(mouse_input.buttons & MIDDLE_CLICK)
        {

        }
        else if(mouse_input.buttons & RIGHT_CLICK)
        {
            mouse_position_on_grid.x = (int)(mouse_input.position.x / grid_layer.cell_size);
            mouse_position_on_grid.y = (int)(mouse_input.position.y / grid_layer.cell_size);
            grass_field_toggle_alive_at(&grass_pool, mouse_position_on_grid, false);
        }

        // !Input Events

        // Update
        sheep_flock_update(&sheep_pool);
        grass_field_update(&grass_pool);
        
        // !Update

        // Render
        SDL_SetRenderDrawColor(renderer,
                               background_color.r,
                               background_color.g,
                               background_color.b,
                               background_color.a);
        SDL_RenderClear(renderer);

        grass_field_draw(renderer,
                         &grass_pool);
        sheep_flock_draw(renderer,
                         &sheep_pool);

        grid_draw(renderer, &grid_layer);

        SDL_RenderPresent(renderer);
        // !Render

        SDL_Delay(16);
    }

    grass_field_clean(&grass_pool);
    grid_free_nodes(&grid_layer);

    SDL_Quit();

    return 0;
}