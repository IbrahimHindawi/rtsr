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
#define FOV 1000.0f

int previous_frame_time = 0;

// buffers
#define max_buffer_size 8096
vec3_t vertex_buffer_[max_buffer_size];
vec2_t vertex_buffer[max_buffer_size];
triangle_t tris_buffer[max_buffer_size];

// mesh data
mesh_t mesh = {0};
vec3_t *mesh_verts_handle = NULL;
face_t *mesh_prim_handle = NULL;

// circle data
#define circle_npts 32
Array circle_points = {0};
vec3_t *circle_points_handle = NULL;

Array circle_indices = {0};
face_t *circle_indices_handle = NULL;

// scene data
// vec3_t scene_angle = {0};
vec3_t camera_position = {0};

uint32_t color = 0x00FF00FF;
#define red 0x0000FFFF
#define green 0x00FF00FF
#define blue 0xFF0000FF

void setup(void) {
    camera_position = vec3_new(0.0f, 0.0f, -6.0f);
    color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        printf("Unable to allocate color_buffer memory.");
    }
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    // mesh_verts_handle = array_create(&cube.points, sizeof(vec3_t), numpts);
    // mesh_prim_handle = array_create(&cube.prims, sizeof(face_t), numprims);

    mesh_verts_handle = array_create(&mesh.points, sizeof(vec3_t), 0);
    mesh_prim_handle = array_create(&mesh.prims, sizeof(face_t), 0);
    load_mesh_from_obj("models/pig.obj", &mesh.points, &mesh.prims);
    printf("%lld points\n", mesh.points.length);
    printf("%lld prims\n", mesh.prims.length);

    mesh_verts_handle = mesh.points.data;
    mesh_prim_handle = mesh.prims.data;

    // load vertex & prim buffer
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        vertex_buffer_[pointcount] = mesh_verts_handle[pointcount];
    }

    circle_points_handle = array_create(&circle_points, sizeof(vec3_t), circle_npts);
    create_circle_vertices(circle_points_handle, circle_points.length, 100.0f);
    // array_print_struct(circle_points, circle_points_handle, vec3tfmt(circle_points_handle));

    circle_indices_handle = array_create(&circle_indices, sizeof(face_t), circle_npts);
    create_circle_indices(circle_indices_handle, circle_indices.length);
    // array_print_struct(circle_indices, circle_indices_handle, facetfmt(circle_indices_handle));
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
    mesh.rotation.x += 0.1f;
    mesh.rotation.y += 0.1f;
    mesh.rotation.z += 0.1f;

    // reload vertex buffer
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        mesh_verts_handle[pointcount] = vertex_buffer_[pointcount];
    }

    // transform & project data
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        mesh_verts_handle[pointcount] = vec3_rotate_x(mesh_verts_handle[pointcount], mesh.rotation.x);
        mesh_verts_handle[pointcount] = vec3_rotate_y(mesh_verts_handle[pointcount], mesh.rotation.y);
        mesh_verts_handle[pointcount] = vec3_rotate_z(mesh_verts_handle[pointcount], mesh.rotation.z);
        mesh_verts_handle[pointcount].z += camera_position.z;

        vertex_buffer[pointcount] = perspective_projection(mesh_verts_handle[pointcount], FOV);
        vertex_buffer[pointcount] = vec2_screen_offset(vertex_buffer[pointcount], window_width, window_height);
    }

    // triangulate
    for (int trinum = 0; trinum < mesh.prims.length; trinum++) {
        triangle_t projected_triangle = {0};
        projected_triangle.a = vertex_buffer[mesh_prim_handle[trinum].a - 1];
        projected_triangle.b = vertex_buffer[mesh_prim_handle[trinum].b - 1];
        projected_triangle.c = vertex_buffer[mesh_prim_handle[trinum].c - 1];
        tris_buffer[trinum] = projected_triangle;
    }
    return;
}

void render(void) {
    draw_grid(0x101010FF, 50);

    // draw mesh verts
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        draw_rectangle(red, vertex_buffer[pointcount].x, vertex_buffer[pointcount].y, 2, 2);
    }

    // draw mesh tris
    for (int trinum = 0; trinum < mesh.prims.length; trinum++) {
        draw_triangle(green, tris_buffer[trinum].a.x, tris_buffer[trinum].a.y, 
                             tris_buffer[trinum].b.x, tris_buffer[trinum].b.y, 
                             tris_buffer[trinum].c.x, tris_buffer[trinum].c.y);
    }

    // draw circle points
    //for (int point_count = 0; point_count < circle_points.length; point_count++) {
    //    draw_rectangle(0xFF0000FF, circle_points_handle[point_count].x + 150.0f,
    //                               circle_points_handle[point_count].y + 150.0f, 2, 2);
    //}

    //draw_line(color, 100, 200, 300, 50);
    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(renderer);
    return;
}

int main(int argc, char *argv[]) {
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

    array_destroy(&mesh.points);
    array_destroy(&mesh.prims);

    array_destroy(&circle_points);
    array_destroy(&circle_indices);

    destroy_window();

    printf("\n");
    printf("RTSR: renderer program terminate:\n");
    printf("\n");

    return 0;
}
