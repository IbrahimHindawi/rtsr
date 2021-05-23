#define __USE_MINGW_ANSI_STDIO 1

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "defines.h"
#include "display.h"
#include "vector.h"

bool is_running;

#define number_of_points (9 * 9 * 9)
vec3_t point_cloud[number_of_points];

void setup(void) {
    color_buffer = (uint32_t*) malloc( sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        printf("Unable to allocate memory.");
    }
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    float x, y, z;
    int point_count = 0;
    for (x = -1.0f; x < 1.0f; x += 0.25) {
        for (y = -1.0f; y < 1.0f; y += 0.25) {
            for (z = -1.0f; z < 1.0f; z += 0.25) {
                vec3_t new_vector = new_vec3(x, y, z);
                point_cloud[point_count++] = new_vector;
            }
        }
        /* point_count += 1; */
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    render_color_buffer();
    clear_color_buffer(0x222222FF);
    draw_grid(0xFF0000FF, 50);
    draw_rectangle(0x00FF00FF, 100, 100, 60, 160);
    draw_pixel(0x0000FFFF, 250, 250);

    SDL_RenderPresent(renderer);
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