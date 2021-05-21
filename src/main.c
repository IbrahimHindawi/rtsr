#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define SDL_error -1

static SDL_Window* window;

bool initialize_window(void) {
    int result =  SDL_Init(SDL_INIT_EVERYTHING);
    if (result == SDL_error) {
        char *error_string = "";
        printf("%s\n", SDL_GetErrorMsg(error_string, 256));
        return false;
    }
    window = SDL_CreateWindow("RTSR", 
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    800, 640, 0);
    return true;
}

int main(int argc, char *argv[]) {
    
    initialize_window();
    
    printf("%I64u\n", sizeof(int));

    SDL_Quit();
    return 0;
}