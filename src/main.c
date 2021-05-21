#define __USE_MINGW_ANSI_STDIO 1
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "defines.h"

#define SDL_error -1

static bool is_running = false;
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static uint32_t* color_buffer = NULL;
static int window_width = 800;
static int window_height = 600;

bool initialize_window(void) {
    int32_t result =  SDL_Init(SDL_INIT_EVERYTHING);
    if (result == SDL_error) {
        char *error_string = "";
        printf("%s\n", SDL_GetErrorMsg(error_string, 256));
        return false;
    }
    window = SDL_CreateWindow( "RTSR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Error creating SDL Window!");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer!");
        return false;
    }
    return true;
}
void setup(void) {
    color_buffer = (uint32_t*) malloc( sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        printf("Unable to allocate memory.");
    }
}
void input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            is_running = false;
        }
        break;
    default:
        break;
    }
}
void update(void) {

}
void render(void) {
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main(int argc, char *argv[]) {
    
    is_running = initialize_window();
    setup();
    while(is_running) {
        input();
        update();
        render();
    }
    free(color_buffer);
    destroy_window();
    return 0;
}