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

static SDL_Texture* color_buffer_texture = NULL;
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
    
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    window_width = display_mode.w;
    window_height = display_mode.h;

    window = SDL_CreateWindow( "RTSR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);
    if (!window) {
        fprintf(stderr, "Error creating SDL Window!");
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL Renderer!");
        return false;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    return true;
}
void setup(void) {
    color_buffer = (uint32_t*) malloc( sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        printf("Unable to allocate memory.");
    }
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
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
void render_color_buffer(void) {
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int) (window_width * sizeof(uint32_t)) );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
void clear_color_buffer(uint32_t color) {
    int y, x;
    for (y = 0; y < window_height; y++) {
        for (x = 0; x < window_width; x++) {
            color_buffer[(window_width * y) + x] = color;
        }
    }
}
void draw_grid(uint32_t color, uint32_t grid_size) {
    int y, x;
    for (y = 0; y < window_height; y++) {
        for (x = 0; x < window_width; x++) {
            color_buffer[y * window_width + x] = !(x % grid_size && y % grid_size) * color;
        }
    }
}
void draw_rectangle(uint32_t color, int posx, int posy, int w, int h) {
    int y, x;
    for (y = 0; y < window_height; y++) {
        for (x = 0; x < window_width; x++) {
            /* color_buffer[y * window_width + x] = !(x > posx && x < posx+w && y > posy && y < posy+h) * color; */
            if (x > posx && x < posx+w && y > posy && y < posy+h) {
                color_buffer[y * window_width + x] = color;
            }
            
        }
    }
}
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    render_color_buffer();
    clear_color_buffer(0xFFFF00FF);
    draw_grid(0xFF0000FF, 50);
    draw_rectangle(0x00FF00FF, 100, 100, 60, 160);

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