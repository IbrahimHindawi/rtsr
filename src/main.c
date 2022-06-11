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

#define FPS 30
#define frame_target_time (1000/FPS)
#define FOV 100.0f

int previous_frame_time = 0;

vec3_t render_verts[numverts] = {0};
vec2_t prjctd_verts[numverts] = {0};

float scene_angle = 0.0f;
vec3_t camera_position = {0};

void setup(void) {
    camera_position = vec3_new(0.0f, 0.0f, -5.0f);
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
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
    previous_frame_time = SDL_GetTicks();
    int time_to_wait = frame_target_time - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= frame_target_time) {
        SDL_Delay(time_to_wait);
    }
    scene_angle += 0.1f;

    for (int primnum = 0; primnum < numprims; primnum++) {
        face_t prim = prims[primnum];
        render_verts[primnum * 3 + 0] = points[prim.a - 1];
        render_verts[primnum * 3 + 1] = points[prim.b - 1];
        render_verts[primnum * 3 + 2] = points[prim.c - 1];
        //printf("vtnum: %d\n", primnum);
        //printf("index: %d, vector{%f, %f, %f}\n", prim.a-1, verts[prim.a - 1].x, verts[prim.a - 1].y, verts[prim.a - 1].z);
        //printf("index: %d, vector{%f, %f, %f}\n", prim.b-1, verts[prim.b - 1].x, verts[prim.b - 1].y, verts[prim.b - 1].z);
        //printf("index: %d, vector{%f, %f, %f}\n", prim.c-1, verts[prim.c - 1].x, verts[prim.c - 1].y, verts[prim.c - 1].z);
    }
    for (int vertnum = 0; vertnum < numverts; vertnum++) {
        //points[i] = vec3_scalar_multiply(points[i], 50.0f);
        //printf("point: %d", i);
        render_verts[vertnum] = vec3_rotate_x(render_verts[vertnum], scene_angle);
        render_verts[vertnum] = vec3_rotate_y(render_verts[vertnum], scene_angle);
        render_verts[vertnum] = vec3_rotate_z(render_verts[vertnum], scene_angle);
        render_verts[vertnum].z += camera_position.z;
        prjctd_verts[vertnum] = perspective_projection(render_verts[vertnum], FOV);
        //prjctd_verts[i] = vec2_new(render_verts[i].x * 50.0f, render_verts[i].y * 50.0f);
        //printf("i: %d, vec3{%f, %f, %f}\n\n", i, render_verts[i].x, render_verts[i].y, render_verts[i].z);
        //printf("point: %d\n", i);
        //printf("|_vec2{%f, %f}\n", prjctd_verts[i].x, prjctd_verts[i].y);
        //printf("|_vec2{%f, %f}\n", render_verts[i].x, render_verts[i].y);
        //printf("|_vec3{%f, %f, %f}\n\n", render_verts[i].x, render_verts[i].y, render_verts[i].z);
    }
}
void render(void) {
    draw_grid(0x101010FF, 50);
    for (int vertnum = 0; vertnum < numverts; vertnum++) {
        draw_rectangle(0x00FF00FF, prjctd_verts[vertnum].x + (window_width * 0.5), prjctd_verts[vertnum].y + (window_height * 0.5), 2, 2);
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
