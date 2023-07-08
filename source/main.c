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

#include "Array.h"

#define FPS 30
#define frame_target_time (1000/FPS)
#define FOV 100.0f

int previous_frame_time = 0;

vec3_t render_verts[numverts] = {0};
vec2_t prjctd_verts[numverts] = {0};
// triangle_t render_tris[numprims] = {0};
Array triangles_array = {0};
triangle_t *render_tris = NULL;

float scene_angle = 0.0f;
vec3_t camera_position = {0};

uint32_t color = 0x00FF00FF;

void setup(void) {
    camera_position = vec3_new(0.0f, 0.0f, -3.0f);
    color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        printf("Unable to allocate color_buffer memory.");
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
    render_tris = array_create(&triangles_array, sizeof(triangle_t), 0);

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
    }
    for (int vertnum = 0; vertnum < numverts; vertnum++) {
        render_verts[vertnum] = vec3_rotate_x(render_verts[vertnum], scene_angle);
        render_verts[vertnum] = vec3_rotate_y(render_verts[vertnum], scene_angle);
        render_verts[vertnum] = vec3_rotate_z(render_verts[vertnum], scene_angle);
        render_verts[vertnum].z += camera_position.z;
        prjctd_verts[vertnum] = perspective_projection(render_verts[vertnum], FOV);
    }
    for (int trinum = 0; trinum < numprims; trinum++) {
        // render_tris[trinum].a = prjctd_verts[trinum * 3 + 0];
        // render_tris[trinum].b = prjctd_verts[trinum * 3 + 1];
        // render_tris[trinum].c = prjctd_verts[trinum * 3 + 2];
        triangle_t projected_triangle = {0};
        projected_triangle.a = prjctd_verts[trinum * 3 + 0];
        projected_triangle.b = prjctd_verts[trinum * 3 + 1];
        projected_triangle.c = prjctd_verts[trinum * 3 + 2];
        render_tris = array_append(&triangles_array, &projected_triangle);
    }
    printf("array length: %zd\n", triangles_array.length);
    for (int trinum = 0; trinum < numprims; trinum++) {
        render_tris[trinum].a.x += (window_width * 0.5);
        render_tris[trinum].a.y += (window_height * 0.5);
        render_tris[trinum].b.x += (window_width * 0.5);
        render_tris[trinum].b.y += (window_height * 0.5);
        render_tris[trinum].c.x += (window_width * 0.5);
        render_tris[trinum].c.y += (window_height * 0.5);
    }
}

void render(void) {
    draw_grid(0x101010FF, 50);
    for (int primnum = 0; primnum < numprims; primnum++) {
        draw_rectangle(color, render_tris[primnum].a.x, render_tris[primnum].a.y, 2, 2);
        draw_rectangle(color, render_tris[primnum].b.x, render_tris[primnum].b.y, 2, 2);
        draw_rectangle(color, render_tris[primnum].c.x, render_tris[primnum].c.y, 2, 2);
        draw_triangle(color, render_tris[primnum].a.x, render_tris[primnum].a.y, 
                             render_tris[primnum].b.x, render_tris[primnum].b.y, 
                             render_tris[primnum].c.x, render_tris[primnum].c.y);

    }
    //draw_line(color, 100, 200, 300, 50);
    array_destroy(&triangles_array);
    render_tris = NULL;
    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(renderer);
}

#ifdef _MSC_VER
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
#else
int main(int argc, char *argv[]) {
#endif
    printf("renderer program start\n");
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
