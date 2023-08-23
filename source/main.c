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

#include "hkArray.h"

#define FPS 30
#define frame_target_time (1000/FPS)
#define FOV 1000.0f

int previous_frame_time = 0;

// buffers
#define max_buffer_size 8192
vec3_t model_vertex_buffer[max_buffer_size];
vec2_t projc_vertex_buffer[max_buffer_size];
// triangle_t tris_buffer[max_buffer_size];

hkArray tris_buffer = {0};
triangle_t *tris_buffer_handle = NULL;

// mesh data
mesh_t mesh = {0};
vec3_t *mesh_verts_handle = NULL;
face_t *mesh_prim_handle = NULL;

// circle data
#define circle_npts 32
hkArray circle_points = {0};
vec3_t *circle_points_handle = NULL;

hkArray circle_indices = {0};
face_t *circle_indices_handle = NULL;

// scene data
// vec3_t scene_angle = {0};
vec3_t camera_position = {0};

uint32_t color = 0x00FF00FF;
#define red 0x0000FFFF
#define green 0x00FF00FF
#define blue 0xFF0000FF

#define u32 uint32_t 

void setup(void) {

    // camera_position = vec3_new(0.0f, 0.0f, 6.0f);
    color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
    if (color_buffer == NULL) {
        printf("Unable to allocate color_buffer memory.");
    }
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
    u32 hello = 0;


    // mesh_verts_handle = array_create(&cube.points, sizeof(vec3_t), numpts);
    // mesh_prim_handle = array_create(&cube.prims, sizeof(face_t), numprims);
    // load mesh data from file into RAM
    // hkArray tris_buffer = hkArrayCreate(sizeof(triangle_t), 0); 
    mesh.points = hkArrayCreate(sizeof(vec3_t), 0);
    mesh.prims = hkArrayCreate(sizeof(face_t), 0);
    load_mesh_from_obj("models/Ecrevisse.obj", &mesh.points, &mesh.prims);

    // load_mesh_from_obj("models/cube.obj", &mesh.points, &mesh.prims);
    printf("%lld points\n", mesh.points.length);
    printf("%lld prims\n", mesh.prims.length);

    mesh_verts_handle = mesh.points.data;
    mesh_prim_handle = mesh.prims.data;

    // load vertex
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        model_vertex_buffer[pointcount] = mesh_verts_handle[pointcount];
    }

    circle_points = hkArrayCreate(sizeof(vec3_t), circle_npts);
    circle_points_handle = circle_points.data;
    create_circle_vertices(circle_points_handle, circle_points.length, 100.0f);
    // array_print_struct(circle_points, circle_points_handle, vec3tfmt(circle_points_handle));

    circle_indices = hkArrayCreate(sizeof(face_t), circle_npts);
    circle_indices_handle = circle_indices.data;
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
    mesh.rotation.x += 0.01f;
    mesh.rotation.y += 0.01f;
    mesh.rotation.z += 0.01f;

    // reload vertex buffer
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        mesh_verts_handle[pointcount] = model_vertex_buffer[pointcount];
    }

    // transform & project data
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        mesh_verts_handle[pointcount] = vec3_rotate_x(mesh_verts_handle[pointcount], mesh.rotation.x);
        mesh_verts_handle[pointcount] = vec3_rotate_y(mesh_verts_handle[pointcount], mesh.rotation.y);
        mesh_verts_handle[pointcount] = vec3_rotate_z(mesh_verts_handle[pointcount], mesh.rotation.z);
        // mesh_verts_handle[pointcount].z -= camera_position.z;
        mesh_verts_handle[pointcount].z += 5.0f;

        projc_vertex_buffer[pointcount] = perspective_projection(mesh_verts_handle[pointcount], FOV);
        projc_vertex_buffer[pointcount] = vec2_screen_offset(projc_vertex_buffer[pointcount], window_width * 0.5f, window_height * 0.5f);
    }

    // initialize tris buffer array
    tris_buffer = hkArrayCreate(sizeof(triangle_t), 0);
    for (int trinum = 0; trinum < mesh.prims.length; trinum++) {
        prim_t primitive = {0};
        primitive.a = mesh_verts_handle[mesh_prim_handle[trinum].a - 1];
        primitive.b = mesh_verts_handle[mesh_prim_handle[trinum].b - 1];
        primitive.c = mesh_verts_handle[mesh_prim_handle[trinum].c - 1];

        // backface culling
        vec3_t ab = vec3_sub(primitive.b, primitive.a);
        vec3_t ac = vec3_sub(primitive.c, primitive.a);
        vec3_t n = vec3_cross(ab, ac);
        vec3_t cam_ray = vec3_sub(camera_position, primitive.a);
        float dot_n_cam = vec3_dot(n, cam_ray);
        if (dot_n_cam > 0.0f) {
            triangle_t projected_triangle = {0};
            projected_triangle.a = projc_vertex_buffer[mesh_prim_handle[trinum].a - 1];
            projected_triangle.b = projc_vertex_buffer[mesh_prim_handle[trinum].b - 1];
            projected_triangle.c = projc_vertex_buffer[mesh_prim_handle[trinum].c - 1];
            tris_buffer_handle = hkArrayAppend(&tris_buffer, &projected_triangle);
        }
    }

    // triangulate
    // for (int trinum = 0; trinum < mesh.prims.length; trinum++) {
    //     triangle_t projected_triangle = {0};
    //     projected_triangle.a = projc_vertex_buffer[mesh_prim_handle[trinum].a - 1];
    //     projected_triangle.b = projc_vertex_buffer[mesh_prim_handle[trinum].b - 1];
    //     projected_triangle.c = projc_vertex_buffer[mesh_prim_handle[trinum].c - 1];
    //     tris_buffer[trinum] = projected_triangle;
    // }
    return;
}

void render(void) {
    draw_grid(0x101010FF, 50);

    // draw mesh verts
    for (int pointcount = 0; pointcount < mesh.points.length; pointcount++) {
        draw_rectangle(blue, projc_vertex_buffer[pointcount].x, projc_vertex_buffer[pointcount].y, 2, 2);
    }

    for (int trinum = 0; trinum < tris_buffer.length; trinum++) {
        draw_triangle(green, tris_buffer_handle[trinum].a.x, tris_buffer_handle[trinum].a.y, 
                             tris_buffer_handle[trinum].b.x, tris_buffer_handle[trinum].b.y, 
                             tris_buffer_handle[trinum].c.x, tris_buffer_handle[trinum].c.y);
    }

    // draw mesh tris
    // for (int trinum = 0; trinum < mesh.prims.length; trinum++) {
    //     draw_triangle(green, tris_buffer_handle[trinum].a.x, tris_buffer_handle[trinum].a.y, 
    //                          tris_buffer_handle[trinum].b.x, tris_buffer_handle[trinum].b.y, 
    //                          tris_buffer_handle[trinum].c.x, tris_buffer_handle[trinum].c.y);
    // }

    // draw circle points
    //for (int point_count = 0; point_count < circle_points.length; point_count++) {
    //    draw_rectangle(0xFF0000FF, circle_points_handle[point_count].x + 150.0f,
    //                               circle_points_handle[point_count].y + 150.0f, 2, 2);
    //}
    //draw_line(color, 100, 200, 300, 50);

    // destroy tris buffer array
    hkArrayDestroy(&tris_buffer);

    render_color_buffer();
    clear_color_buffer(0x000000FF);
    SDL_RenderPresent(renderer);
    return;
}

int main(int argc, char *argv[]) {
    // vector_test();

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

    hkArrayDestroy(&mesh.points);
    hkArrayDestroy(&mesh.prims);

    hkArrayDestroy(&circle_points);
    hkArrayDestroy(&circle_indices);

    destroy_window();

    printf("\n");
    printf("RTSR: renderer program terminate:\n");
    printf("\n");

    return 0;
}
