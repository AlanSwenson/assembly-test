#ifndef DECK_H
#define DECK_H

#include "cards.h"
#include <stdbool.h>

// Maximum deck size
#define MAX_DECK_SIZE 50
#define MAX_HAND_SIZE 10

// Deck structure
typedef struct {
    Card draw_pile[MAX_DECK_SIZE];
    int draw_pile_count;
    
    Card discard_pile[MAX_DECK_SIZE];
    int discard_pile_count;
    
    Card hand[MAX_HAND_SIZE];
    int hand_count;
} Deck;

// Deck management functions
void init_deck(Deck* deck);

bool add_card_to_deck(Deck* deck, const Card* card);

Card* draw_card(Deck* deck);

void discard_card(Deck* deck, int hand_index);

void shuffle_deck(Deck* deck);

void reshuffle_discard_into_draw(Deck* deck);

// Deck information functions
int get_deck_total_count(const Deck* deck);
int get_draw_pile_count(const Deck* deck);
int get_discard_pile_count(const Deck* deck);
int get_hand_count(const Deck* deck);

// Starter deck creation
void create_starter_deck(Deck* deck, const CardPool* pool);

#endif // DECK_H
