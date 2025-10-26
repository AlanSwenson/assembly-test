#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    if (TTF_Init() == -1) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("Text Test",
                                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         WINDOW_WIDTH, WINDOW_HEIGHT,
                                         SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    
    // Try to load font
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/Arial.ttf", 24);
    if (!font) {
        font = TTF_OpenFont("/Library/Fonts/Arial.ttf", 24);
    }
    if (!font) {
        printf("Could not load font, using fallback\n");
    } else {
        printf("Font loaded successfully!\n");
    }
    
    bool running = true;
    SDL_Event event;
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }
        
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        // Draw some colored rectangles as text
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color red = {255, 0, 0, 255};
        SDL_Color green = {0, 255, 0, 255};
        SDL_Color blue = {0, 0, 255, 255};
        
        // Draw "HELLO WORLD" as rectangles
        const char* text = "HELLO WORLD";
        int x = 100, y = 100;
        int char_width = 15, char_height = 20;
        
        for (int i = 0; text[i] != '\0'; i++) {
            if (text[i] == ' ') {
                x += char_width;
                continue;
            }
            
            SDL_Rect char_rect = {x, y, char_width, char_height};
            SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
            SDL_RenderFillRect(renderer, &char_rect);
            
            // Add border
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &char_rect);
            
            x += char_width;
        }
        
        // Draw "TEST" in red
        const char* test_text = "TEST";
        x = 100; y = 150;
        for (int i = 0; test_text[i] != '\0'; i++) {
            SDL_Rect char_rect = {x, y, char_width, char_height};
            SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
            SDL_RenderFillRect(renderer, &char_rect);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &char_rect);
            
            x += char_width;
        }
        
        // Draw "GAME" in green
        const char* game_text = "GAME";
        x = 100; y = 200;
        for (int i = 0; game_text[i] != '\0'; i++) {
            SDL_Rect char_rect = {x, y, char_width, char_height};
            SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
            SDL_RenderFillRect(renderer, &char_rect);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &char_rect);
            
            x += char_width;
        }
        
        // Draw instructions
        const char* instructions = "PRESS ESC TO QUIT";
        x = 100; y = 300;
        for (int i = 0; instructions[i] != '\0'; i++) {
            if (instructions[i] == ' ') {
                x += char_width;
                continue;
            }
            
            SDL_Rect char_rect = {x, y, char_width, char_height};
            SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, blue.a);
            SDL_RenderFillRect(renderer, &char_rect);
            
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &char_rect);
            
            x += char_width;
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    if (font) {
        TTF_CloseFont(font);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}
