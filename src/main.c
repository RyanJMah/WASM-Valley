#include <stdint.h>
#include <SDL2/SDL.h>
#include "macros.h"

#ifdef WASM
#include <emscripten.h>
#endif

static SDL_Window* window;   // Declare a window
static SDL_Renderer* renderer;  // Declare a renderer

#ifndef WASM
    #define FRAME_RATE          ( 60 )
    #define FRAME_TIME_TICKS    ( 1000 / FRAME_RATE )

    static uint32_t g_quit = 0;
#endif

void main_loop()
{
    SDL_Event e;
    if ( SDL_PollEvent(&e) )
    {
        switch (e.type)
        {
            case SDL_QUIT:
            {
            #ifdef WASM
                emscripten_cancel_main_loop();
            #else
                g_quit = 1;
            #endif

                break;
            }

        }
    }


    // Set the draw color to white
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // Clear the entire screen to our selected color
    SDL_RenderClear(renderer);

    // Up until now everything was drawn behind the scenes.
    // This will show the new, white contents of the window.
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[])
{
    // Initialize SDL2
    SDL_Init(SDL_INIT_VIDEO);

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "Hello World",         // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        640,                    // width, in pixels
        480,                    // height, in pixels
        0                       // flags
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

#ifdef WASM
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while ( !g_quit )
    {
        uint32_t start = SDL_GetTicks();
        main_loop();
        uint32_t end = SDL_GetTicks();

        uint32_t frame_time = end - start;

        if ( frame_time < FRAME_TIME_TICKS )
        {
            SDL_Delay(FRAME_TIME_TICKS - frame_time);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
#endif

    return 0;
}
