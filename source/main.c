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
#include "geometry.h"

#include "Array.h"

#define FPS 30
#define frame_target_time (1000/FPS)
#define FOV 100.0f

int previous_frame_time = 0;

// cube data
vec3_t cube_raw_verts[numverts] = {0};
vec2_t cube_ren_verts[numverts] = {0};
Array cube_tris_array = {0};
triangle_t *cube_tris_handle = NULL;

// circle data
#define circle_npts 32
Array circle_points = {0};
vec3_t *circle_points_handle = NULL;

Array circle_indices = {0};
face_t *circle_indices_handle = NULL;

// scene data
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

    circle_points_handle = array_create(&circle_points, sizeof(vec3_t), circle_npts);
    create_circle_vertices(circle_points_handle, circle_points.length, 100.0f);
    array_print_struct(circle_points, circle_points_handle, vec3tfmt(circle_points_handle));

    circle_indices_handle = array_create(&circle_indices, sizeof(face_t), circle_npts);
    create_circle_indices(circle_indices_handle, circle_indices.length);
    array_print_struct(circle_indices, circle_indices_handle, facetfmt(circle_indices_handle));
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
    cube_tris_handle = array_create(&cube_tris_array, sizeof(triangle_t), 0);

    previous_frame_time = SDL_GetTicks();
    int time_to_wait = frame_target_time - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait <= frame_target_time) {
        SDL_Delay(time_to_wait);
    }
    scene_angle += 0.1f;

    // reload vertex buffer
    for (int primnum = 0; primnum < numprims; primnum++) {
        face_t prim = cube_prims[primnum];
        cube_raw_verts[primnum * 3 + 0] = cube_points[prim.a - 1];
        cube_raw_verts[primnum * 3 + 1] = cube_points[prim.b - 1];
        cube_raw_verts[primnum * 3 + 2] = cube_points[prim.c - 1];
    }

    // transform & project data
    for (int vertnum = 0; vertnum < numverts; vertnum++) {
        cube_raw_verts[vertnum] = vec3_rotate_x(cube_raw_verts[vertnum], scene_angle);
        cube_raw_verts[vertnum] = vec3_rotate_y(cube_raw_verts[vertnum], scene_angle);
        cube_raw_verts[vertnum] = vec3_rotate_z(cube_raw_verts[vertnum], scene_angle);
        cube_raw_verts[vertnum].z += camera_position.z;

        cube_ren_verts[vertnum] = perspective_projection(cube_raw_verts[vertnum], FOV);
        cube_ren_verts[vertnum] = vec2_screen_centralize(cube_ren_verts[vertnum], window_width, window_height);
    }

    // triangulate
    for (int trinum = 0; trinum < numprims; trinum++) {
        triangle_t projected_triangle = {0};
        projected_triangle.a = cube_ren_verts[trinum * 3 + 0];
        projected_triangle.b = cube_ren_verts[trinum * 3 + 1];
        projected_triangle.c = cube_ren_verts[trinum * 3 + 2];
        cube_tris_handle = array_append(&cube_tris_array, &projected_triangle);
    }
}

void render(void) {
    draw_grid(0x101010FF, 50);
    for (int primnum = 0; primnum < numprims; primnum++) {
        draw_rectangle(color, cube_tris_handle[primnum].a.x, cube_tris_handle[primnum].a.y, 2, 2);
        draw_rectangle(color, cube_tris_handle[primnum].b.x, cube_tris_handle[primnum].b.y, 2, 2);
        draw_rectangle(color, cube_tris_handle[primnum].c.x, cube_tris_handle[primnum].c.y, 2, 2);
        draw_triangle(color, cube_tris_handle[primnum].a.x, cube_tris_handle[primnum].a.y, 
                             cube_tris_handle[primnum].b.x, cube_tris_handle[primnum].b.y, 
                             cube_tris_handle[primnum].c.x, cube_tris_handle[primnum].c.y);

    }
    for (int point_count = 0; point_count < circle_points.length; point_count++) {
        draw_rectangle(0xFF0000FF, circle_points_handle[point_count].x + 150.0f,
                                   circle_points_handle[point_count].y + 150.0f, 2, 2);

    }
    //draw_line(color, 100, 200, 300, 50);
    array_destroy(&cube_tris_array);
    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(renderer);
}

#ifdef _MSC_VER
// int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
int main(int argc, char *argv[]) {
#else
int main(int argc, char *argv[]) {
#endif
    printf("\n");
    printf("RTSR: renderer program start:\n");
    printf("\n");

    is_running = initialize_window();
    setup();
    while(is_running) {
        input();
        update();
        render();
    }
    free(color_buffer);
    array_destroy(&circle_points);
    array_destroy(&circle_indices);
    destroy_window();

    printf("\n");
    printf("RTSR: renderer program terminate:\n");
    printf("\n");

    return 0;
}
