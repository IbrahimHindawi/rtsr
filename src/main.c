#define __USE_MINGW_ANSI_STDIO 1
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "defines.h"
#include "display.h"

bool is_running;

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
void render(void) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    render_color_buffer();
    clear_color_buffer(0xFFFF00FF);
    draw_grid(0xFF0000FF, 50);
    draw_rectangle(0x00FF00FF, 100, 100, 60, 160);

    SDL_RenderPresent(renderer);
}

typedef struct {
    float x;
    float y;
} vec2_t;

int main(int argc, char *argv[]) {
    vec2_t v1 = {0};
    vec2_t v2 = {1.0, 3.0};

    printf("%f:%f\n", v1.x, v1.y);
    printf("%llu", sizeof(v1));

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