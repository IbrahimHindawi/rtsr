#include "display.h"
#include "vector.h"

#define SDL_error -1

bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* color_buffer_texture = NULL;
uint32_t* color_buffer = NULL;
int window_width = 0;
int window_height = 0;

/*////////////////////////////////////////////////////////////////// 
 * Window
 *//////////////////////////////////////////////////////////////////
bool initialize_window(void) {
    int32_t result =  SDL_Init(SDL_INIT_EVERYTHING);
    if (result == SDL_error) {
        //char *error_string = "";
        //printf("%s\n", SDL_GetErrorMsg(error_string, 256));
        return false;
    }
    
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    //window_width = display_mode.w;
    //window_height = display_mode.h;

    window_width = 1152;
    window_height = 720;

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
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    return true;
}

void destroy_window(void) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
/*////////////////////////////////////////////////////////////////// 
 * Render
 *//////////////////////////////////////////////////////////////////

void render_color_buffer(void) {
    SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int) (window_width * sizeof(uint32_t)) );
    SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
    int y, x;
    for (y = 0; y < window_height; y++) {
        for (x = 0; x < window_width; x++) {
            color_buffer[window_width * y + x] = color;
        }
    }
}

void draw_pixel(uint32_t color, int x, int y) {
    if ( x >= 0 && x < window_width && y >= 0 && window_height) {
    	color_buffer[window_width * y + x] = color;
	}
}

void draw_grid(uint32_t color, uint32_t grid_size) {
    int y, x;
    for (y = 0; y < window_height; y++) {
        for (x = 0; x < window_width; x++) {
            if ( !(x % grid_size && y % grid_size) ) {
                draw_pixel(color, x, y);
            }
        }
    }
}

void draw_rectangle(uint32_t color, int posx, int posy, int w, int h) {
	for(int x = posx; x < w + posx; x++) {
		for(int y = posy; y < h + posy; y++) {
			if ( x >= 0 && x < window_width && y >= 0 && y < window_height) {
				draw_pixel(color, x, y);
			}
		}
	}
}

void draw_line(uint32_t color, int x0, int y0, int x1, int y1) {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);
    int longest = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);
    float x_inc = delta_x / (float)longest;
    float y_inc = delta_y / (float)longest;
    float cur_x = x0;
    float cur_y = y0;
    for(int step = 0; step <= longest; step++) {
        draw_pixel(color, round(cur_x), round(cur_y));
        cur_x += x_inc;
        cur_y += y_inc;
    }
}

void draw_triangle(uint32_t color, int x0, int y0, int x1, int y1, int x2, int y2) {
    draw_line(color, x0, y0, x1, y1);
    draw_line(color, x1, y1, x2, y2);
    draw_line(color, x2, y2, x0, y0);
}
