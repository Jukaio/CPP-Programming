
#pragma comment(lib, "../Dependencies/SDL2/lib/x64/SDL2.lib")
#pragma comment(lib, "../Dependencies/SDL2/lib/x64/SDL2main.lib")
#pragma comment(lib, "../Dependencies/SDL2_mixer/lib/x64/SDL2_mixer.lib")
#pragma comment(lib, "../Dependencies/SDL2_image/lib/x64/SDL2_image.lib")

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <chrono>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include "zelda.h"

int main(int ac, char** av)
{
	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL: %s", SDL_GetError());

	// Initialise SDL_Mixer (Sound and Music)
	if(Mix_Init(MIX_INIT_MP3) == 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize SDL_mixer: %s", Mix_GetError());
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize OpenAudio: %s", Mix_GetError());
	
	// Initialise SDL_Image (Enabled the loading of PNG and JPG)
	if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not initialize IMG: %s", IMG_GetError());

	// Create a window - Maybe its own function?
	SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CONFIG::SCREEN_WIDTH, CONFIG::SCREEN_HEIGHT, 0);
	if(window == nullptr)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create SDL_Window: %s", SDL_GetError());
	
	// Creates a renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not create SDL_Renderer: %s", SDL_GetError());

	SDL_RenderSetScale(renderer, CONFIG::SCALE, CONFIG::SCALE);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// Creates the game
	// zelda is a derived class of game
	// I decided to do it this way due to "reusability" in the future
	// i.e. game* g = new mario(renderer) or new doom(renderer), etc.
	game* g = new zelda(renderer);

	auto now = std::chrono::system_clock::now().time_since_epoch().count();
	srand(now);

	g->enter();
	g->run();
	g->exit();
	
	delete g;

	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////

	// Destroys renderer
	if (renderer != nullptr)
		SDL_DestroyRenderer(renderer);

	// Destroys window
	if (window != nullptr)
		SDL_DestroyWindow(window);

	// Quits SDL_Image
	IMG_Quit();

	// Quits SDL_Mixer
	Mix_CloseAudio();
	Mix_Quit();

	// Quits SDL
	SDL_Quit();

	// Checks for memory leaks
	_CrtDumpMemoryLeaks();

	return 0;
}