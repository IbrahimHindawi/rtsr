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
#define FOV 10.0f

int previous_frame_time = 0;

vec3_t render_points[numverts];
vec2_t projected_points[numverts];

float scene_angle = 0.0f;
vec3_t camera_position;

void setup(void) {
    camera_position = vec3_new(0.0f, 0.0f, -5.0f);
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
    //int vtcount = 0;
    for (int primnum = 0; primnum < numprims; primnum++) {
        face_t prim = prims[primnum];
        render_points[0 + (primnum * 3)] = verts[prim.a - 1];
        render_points[1 + (primnum * 3)] = verts[prim.b - 1];
        render_points[2 + (primnum * 3)] = verts[prim.c - 1];
        //printf("vtnum: %d\n", primnum);
        //printf("index: %d, vector{%f, %f, %f}\n", prim.a-1, verts[prim.a - 1].x, verts[prim.a - 1].y, verts[prim.a - 1].z);
        //printf("index: %d, vector{%f, %f, %f}\n", prim.b-1, verts[prim.b - 1].x, verts[prim.b - 1].y, verts[prim.b - 1].z);
        //printf("index: %d, vector{%f, %f, %f}\n", prim.c-1, verts[prim.c - 1].x, verts[prim.c - 1].y, verts[prim.c - 1].z);
    }
    for (int i = 0; i < numverts; i++) {
        //points[i] = vec3_scalar_multiply(points[i], 50.0f);
        //printf("point: %d", i);
		render_points[i] = vec3_rotate_x(render_points[i], scene_angle);
		render_points[i] = vec3_rotate_y(render_points[i], scene_angle);
		render_points[i] = vec3_rotate_z(render_points[i], scene_angle);
		render_points[i].z += camera_position.z;
        projected_points[i] = perspective_projection(render_points[i], 100.0f);
        //projected_points[i] = vec2_new(render_points[i].x * 50.0f, render_points[i].y * 50.0f);
        //printf("i: %d, vec3{%f, %f, %f}\n\n", i, render_points[i].x, render_points[i].y, render_points[i].z);
        //printf("point: %d\n", i);
        //printf("|_vec2{%f, %f}\n", projected_points[i].x, projected_points[i].y);
        printf("|_vec2{%f, %f}\n", render_points[i].x, render_points[i].y);
        printf("|_vec3{%f, %f, %f}\n\n", render_points[i].x, render_points[i].y, render_points[i].z);
    }
}
void render(void) {
    draw_grid(0x101010FF, 50);

/*
    for (int pointnum = 0; pointnum < number_of_points; pointnum++) {
        draw_rectangle(0x00FF00FF, prjct_ptcld[pointnum].x + (window_width * 0.5), prjct_ptcld[pointnum].y + (window_height * 0.5), 4, 4);
    }
*/    
    for (int vtcount = 0; vtcount < numverts; vtcount++) {
        //printf("projected_vector2{%f, %f}\n", projected_points[vtcount].x, projected_points[vtcount].y);
        draw_rectangle(0x00FF00FF, projected_points[vtcount].x + (window_width * 0.5), projected_points[vtcount].y + (window_height * 0.5), 2, 2);
    }
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
