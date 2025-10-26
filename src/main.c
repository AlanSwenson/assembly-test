#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "player.h"
#include "enemy.h"
#include "cards.h"
#include "deck.h"
#include "combat.h"
#include "random.h"

// Window dimensions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Colors
#define COLOR_BLACK (SDL_Color){0, 0, 0, 255}
#define COLOR_WHITE (SDL_Color){255, 255, 255, 255}
#define COLOR_RED (SDL_Color){255, 0, 0, 255}
#define COLOR_GREEN (SDL_Color){0, 255, 0, 255}
#define COLOR_BLUE (SDL_Color){0, 0, 255, 255}
#define COLOR_GRAY (SDL_Color){128, 128, 128, 255}
#define COLOR_DARK_GRAY (SDL_Color){64, 64, 64, 255}

// Game state
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    bool running;
    Player player;
    Enemy enemy;
    Deck deck;
    CardPool card_pool;
    Combat combat;
    int selected_card;
} GameState;

// Initialize SDL2 and create window
bool init_sdl(GameState* game) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    if (TTF_Init() == -1) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        return false;
    }
    
    game->window = SDL_CreateWindow("Deckbuilder Roguelike",
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT,
                                   SDL_WINDOW_SHOWN);
    if (!game->window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
    if (!game->renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    // Load font (using system font as fallback)
    // Try macOS paths first
    game->font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 24);
    if (!game->font) {
        game->font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Times New Roman.ttf", 24);
    }
    if (!game->font) {
        game->font = TTF_OpenFont("/System/Library/Fonts/Times.ttc", 24);
    }
    // Try Linux paths
    if (!game->font) {
        printf("Trying alternative font paths...\n");
        game->font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
    }
    if (!game->font) {
        printf("Warning: Could not load any font, using fallback rendering\n");
    } else {
        printf("Font loaded successfully!\n");
    }
    
    return true;
}

// Render text using simple bitmap font
void render_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, 
                 int x, int y, SDL_Color color) {
    if (!text) return;
    
    // Try to use TTF font first
    if (font) {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture) {
                SDL_Rect dest = {x, y, surface->w, surface->h};
                SDL_RenderCopy(renderer, texture, NULL, &dest);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(surface);
            return;
        }
    }
    
    // Fallback: render text as colored rectangles with better spacing
    int char_width = 20;
    int char_height = 30;
    
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ') {
            x += char_width;
            continue;
        }
        
        // Draw a simple rectangle for each character with better visibility
        SDL_Rect char_rect = {x, y, char_width, char_height};
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &char_rect);
        
        // Add a thick border to make characters more distinct
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &char_rect);
        
        // Add inner border for extra visibility
        SDL_Rect inner_rect = {x+1, y+1, char_width-2, char_height-2};
        SDL_RenderDrawRect(renderer, &inner_rect);
        
        x += char_width;
    }
}

// Render a card
void render_card(SDL_Renderer* renderer, TTF_Font* font, const Card* card, 
                 int x, int y, bool selected) {
    SDL_Color bg_color = selected ? COLOR_GRAY : COLOR_WHITE;
    SDL_Color text_color = COLOR_BLACK;
    
    // Card background
    SDL_Rect card_rect = {x, y, 200, 250};
    SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
    SDL_RenderFillRect(renderer, &card_rect);
    
    // Card border (thicker if selected)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &card_rect);
    if (selected) {
        SDL_Rect inner_rect = {x+2, y+2, 196, 246};
        SDL_RenderDrawRect(renderer, &inner_rect);
    }
    
    // Card name
    render_text(renderer, font, card->name, x + 10, y + 10, text_color);
    
    // Card cost
    char cost_text[32];
    snprintf(cost_text, sizeof(cost_text), "Cost: %d", card->cost);
    render_text(renderer, font, cost_text, x + 10, y + 50, text_color);
    
    // Card effects
    int y_offset = 90;
    if (card->damage > 0) {
        char damage_text[32];
        snprintf(damage_text, sizeof(damage_text), "DMG: %d", card->damage);
        render_text(renderer, font, damage_text, x + 10, y + y_offset, COLOR_RED);
        y_offset += 40;
    }
    
    if (card->block > 0) {
        char block_text[32];
        snprintf(block_text, sizeof(block_text), "BLK: %d", card->block);
        render_text(renderer, font, block_text, x + 10, y + y_offset, COLOR_BLUE);
        y_offset += 40;
    }
    
    if (card->heal > 0) {
        char heal_text[32];
        snprintf(heal_text, sizeof(heal_text), "HEAL: %d", card->heal);
        render_text(renderer, font, heal_text, x + 10, y + y_offset, COLOR_GREEN);
        y_offset += 40;
    }
}

// Render health bar
void render_health_bar(SDL_Renderer* renderer, int x, int y, int width, int height,
                       int current_hp, int max_hp, const char* name) {
    // Background
    SDL_Rect bg_rect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
    SDL_RenderFillRect(renderer, &bg_rect);
    
    // Health bar
    int health_width = (current_hp * width) / max_hp;
    if (health_width > 0) {
        SDL_Rect health_rect = {x, y, health_width, height};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &health_rect);
    }
    
    // Border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &bg_rect);
    
    // Health text
    char health_text[64];
    snprintf(health_text, sizeof(health_text), "%s: %d/%d", name, current_hp, max_hp);
    render_text(renderer, NULL, health_text, x, y - 15, COLOR_WHITE);
}

// Render the game
void render_game(GameState* game) {
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game->renderer);
    
    // Render player health and energy
    render_health_bar(game->renderer, 50, 50, 200, 20, 
                      game->player.hp, game->player.max_hp, "Player");
    
    char energy_text[64];
    snprintf(energy_text, sizeof(energy_text), "Energy: %d/%d", 
             game->player.energy, game->player.max_energy);
    render_text(game->renderer, game->font, energy_text, 50, 80, COLOR_WHITE);
    
    if (game->player.block > 0) {
        char block_text[64];
        snprintf(block_text, sizeof(block_text), "Block: %d", game->player.block);
        render_text(game->renderer, game->font, block_text, 50, 110, COLOR_BLUE);
    }
    
    // Render enemy health
    render_health_bar(game->renderer, 50, 150, 200, 20,
                      game->enemy.hp, game->enemy.max_hp, game->enemy.name);
    
    // Render enemy intent
    const char* intent_text = "Unknown";
    switch (game->enemy.intent) {
        case ENEMY_INTENT_ATTACK: intent_text = "Will Attack"; break;
        case ENEMY_INTENT_DEFEND: intent_text = "Will Defend"; break;
        case ENEMY_INTENT_BUFF: intent_text = "Will Buff"; break;
        default: intent_text = "Thinking..."; break;
    }
    render_text(game->renderer, game->font, intent_text, 50, 180, COLOR_RED);
    
    // Render hand
    render_text(game->renderer, game->font, "Hand:", 50, 250, COLOR_WHITE);
    for (int i = 0; i < game->deck.hand_count; i++) {
        int card_x = 50 + i * 220;
        int card_y = 280;
        bool selected = (i == game->selected_card);
        render_card(game->renderer, game->font, &game->deck.hand[i], card_x, card_y, selected);
    }
    
    // Render instructions
    render_text(game->renderer, game->font, "CONTROLS:", 50, 500, COLOR_WHITE);
    render_text(game->renderer, game->font, "1-5: Select card", 50, 520, COLOR_WHITE);
    render_text(game->renderer, game->font, "SPACE: Play card", 50, 540, COLOR_WHITE);
    render_text(game->renderer, game->font, "ENTER: End turn", 50, 560, COLOR_WHITE);
    render_text(game->renderer, game->font, "ESC: Quit", 50, 580, COLOR_WHITE);
    
    // Render combat state
    const char* state_text = "PLAYER TURN";
    SDL_Color state_color = COLOR_GREEN;
    if (game->combat.state == COMBAT_STATE_ENEMY_TURN) {
        state_text = "ENEMY TURN";
        state_color = COLOR_RED;
    } else if (game->combat.is_finished) {
        state_text = game->combat.result == COMBAT_RESULT_PLAYER_VICTORY ? "VICTORY!" : "DEFEAT!";
        state_color = game->combat.result == COMBAT_RESULT_PLAYER_VICTORY ? COLOR_GREEN : COLOR_RED;
    }
    render_text(game->renderer, game->font, state_text, 50, 620, state_color);
    
    // Render turn number
    char turn_text[32];
    snprintf(turn_text, sizeof(turn_text), "Turn: %d", game->combat.turn_number);
    render_text(game->renderer, game->font, turn_text, 50, 640, COLOR_WHITE);
    
    SDL_RenderPresent(game->renderer);
}

// Handle input
void handle_input(GameState* game) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                game->running = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        game->running = false;
                        break;
                        
                    case SDLK_1:
                    case SDLK_2:
                    case SDLK_3:
                    case SDLK_4:
                    case SDLK_5:
                        game->selected_card = event.key.keysym.sym - SDLK_1;
                        if (game->selected_card >= game->deck.hand_count) {
                            game->selected_card = -1;
                        }
                        break;
                        
                    case SDLK_SPACE:
                        if (game->selected_card >= 0 && game->selected_card < game->deck.hand_count) {
                            if (play_card(&game->combat, &game->deck, game->selected_card)) {
                                game->selected_card = -1;
                                check_combat_end(&game->combat);
                            }
                        }
                        break;
                        
                    case SDLK_RETURN:
                        if (game->combat.state == COMBAT_STATE_PLAYER_TURN) {
                            end_player_combat_turn(&game->combat);
                            execute_enemy_turn(&game->combat);
                            check_combat_end(&game->combat);
                        }
                        break;
                }
                break;
        }
    }
}

// Initialize game
void init_game(GameState* game) {
    // Initialize RNG
    seed_rng(12345);
    
    // Initialize card pool
    init_card_pool(&game->card_pool);
    
    // Initialize player
    init_player(&game->player);
    
    // Initialize enemy
    init_enemy(&game->enemy, ENEMY_CULTIST, "Cultist");
    
    // Initialize deck
    create_starter_deck(&game->deck, &game->card_pool);
    
    // Draw initial hand
    for (int i = 0; i < 5 && i < game->deck.draw_pile_count; i++) {
        draw_card(&game->deck);
    }
    
    // Initialize combat
    init_combat(&game->combat, &game->player, &game->enemy);
    
    game->selected_card = -1;
    game->running = true;
}

// Cleanup
void cleanup(GameState* game) {
    if (game->font) {
        TTF_CloseFont(game->font);
    }
    if (game->renderer) {
        SDL_DestroyRenderer(game->renderer);
    }
    if (game->window) {
        SDL_DestroyWindow(game->window);
    }
    TTF_Quit();
    SDL_Quit();
}

// Main game loop
int main(int argc, char* argv[]) {
    GameState game = {0};
    
    if (!init_sdl(&game)) {
        return 1;
    }
    
    init_game(&game);
    
    printf("Deckbuilder Roguelike - Controls:\n");
    printf("1-5: Select card\n");
    printf("SPACE: Play selected card\n");
    printf("ENTER: End turn\n");
    printf("ESC: Quit\n\n");
    
    while (game.running) {
        handle_input(&game);
        render_game(&game);
        SDL_Delay(16); // ~60 FPS
    }
    
    cleanup(&game);
    return 0;
}
