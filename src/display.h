#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;
extern int window_width;
extern int window_height;

bool initialize_window(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void draw_pixel(uint32_t color, int posx, int posy);
void draw_grid(uint32_t color, uint32_t grid_size);
void draw_rectangle(uint32_t color, int posx, int posy, int w, int h);
void destroy_window(void); 

#endif