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
/*////////////////////////////////////////////////////////////////// 
 * Projection
 *//////////////////////////////////////////////////////////////////
/*
  similar triangles have the same ratios when we divide their sides
  Y
  |.
  |   .
  |      .  C
  |         .
  |         |   .
  |         |      .
  |         |         .
  -----------------------.
  X         B            A

  BC/XY = AB/AX
  if AB = 1 then BC = XY/AX
  else BC = (1/XY) * (AB/AX) then BC = (1*AB) / (XY/AX)
  C.x = Y.x/X.z
  basically: you divide by Z
 */
vec2_t perspective_projection(vec3_t pt, float fov) {
    float x, y;
    x = (pt.x * fov) / pt.z;
    y = (pt.y * fov) / pt.z;
    return vec2_new(x, y);
}
