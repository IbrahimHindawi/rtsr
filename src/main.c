//#define WIN64
#ifdef _MSC_VER
    //#define __USE_MINGW_ANSI_STDIO 1
    #define SDL_MAIN_HANDLED
    #include <windows.h>
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "defines.h"
#include "display.h"
#include "vector.h"
#include "mesh.h"

//extern bool is_running;
#define FPS 30
#define frame_target_time (1000/FPS)

int previous_frame_time = 0;


float scene_angle = 0.0f;
vec3_t camera_position;

void setup(void) {
    camera_position = vec3_new(0.0f, 0.0f, -500.0f);
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
 */
void update(void) {
    previous_frame_time = SDL_GetTicks();
    int time_to_wait = frame_target_time - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= frame_target_time) {
        SDL_Delay(time_to_wait);
    }
	scene_angle += 0.1f;
/*
    for (int ptnum = 0; ptnum < number_of_points; ptnum++) {
        vec3_t pt = point_cloud[ptnum];
		pt = vec3_rotate_x(pt, scene_angle);
		pt = vec3_rotate_y(pt, scene_angle);
		pt = vec3_rotate_z(pt, scene_angle);
        pt.z += camera_position.z;
        prjct_ptcld[ptnum] = perspective_projection(pt, 1000.0f);
    }
*/
}
void render(void) {
    //draw_grid(0x101010FF, 50);

/*
    for (int pointnum = 0; pointnum < number_of_points; pointnum++) {
        draw_rectangle(0x00FF00FF, prjct_ptcld[pointnum].x + (window_width * 0.5), prjct_ptcld[pointnum].y + (window_height * 0.5), 4, 4);
    }
*/    
    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(renderer);
}

#ifdef _MSC_VER
int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {
#else
int main(int argc, char *argv[]) {
#endif
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
