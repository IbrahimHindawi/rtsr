/*
#include <stdio.h>
#include <SDL2/SDL.h>

int main(void) {
    const char* string = "Hell World!";
    printf("%s\n", string);
    return 0;
}
*/

#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Quit();
    return 0;
}