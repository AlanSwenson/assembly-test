#include "deck.h"
#include "random.h"
#include <string.h>

// Initialize an empty deck
void init_deck(Deck* deck) {
    if (!deck) {
        return;
    }
    
    memset(deck, 0, sizeof(Deck));
}

// Add a card to the deck's draw pile
bool add_card_to_deck(Deck* deck, const Card* card) {
    if (!deck || !card || deck->draw_pile_count >= MAX_DECK_SIZE) {
        return false;
    }
    
    deck->draw_pile[deck->draw_pile_count] = *card;
    deck->draw_pile_count++;
    return true;
}

// Draw a card from the draw pile to the hand
Card* draw_card(Deck* deck) {
    if (!deck || deck->hand_count >= MAX_HAND_SIZE) {
        return NULL;
    }
    
    // If draw pile is empty, reshuffle discard pile
    if (deck->draw_pile_count == 0) {
        if (deck->discard_pile_count == 0) {
            return NULL; // No cards to draw
        }
        reshuffle_discard_into_draw(deck);
    }
    
    // Draw the top card
    Card* drawn = &deck->draw_pile[deck->draw_pile_count - 1];
    deck->hand[deck->hand_count] = *drawn;
    deck->hand_count++;
    deck->draw_pile_count--;
    
    return &deck->hand[deck->hand_count - 1];
}

// Discard a card from hand to discard pile
void discard_card(Deck* deck, int hand_index) {
    if (!deck || hand_index < 0 || hand_index >= deck->hand_count || 
        deck->discard_pile_count >= MAX_DECK_SIZE) {
        return;
    }
    
    // Move card from hand to discard pile
    deck->discard_pile[deck->discard_pile_count] = deck->hand[hand_index];
    deck->discard_pile_count++;
    
    // Remove card from hand by shifting remaining cards
    for (int i = hand_index; i < deck->hand_count - 1; i++) {
        deck->hand[i] = deck->hand[i + 1];
    }
    deck->hand_count--;
}

// Shuffle the draw pile using Fisher-Yates algorithm
void shuffle_deck(Deck* deck) {
    if (!deck || deck->draw_pile_count <= 1) {
        return;
    }
    
    // Fisher-Yates shuffle
    for (int i = deck->draw_pile_count - 1; i > 0; i--) {
        uint64_t j = random_range(i + 1);
        
        // Swap cards
        Card temp = deck->draw_pile[i];
        deck->draw_pile[i] = deck->draw_pile[j];
        deck->draw_pile[j] = temp;
    }
}

// Move all cards from discard pile to draw pile and shuffle
void reshuffle_discard_into_draw(Deck* deck) {
    if (!deck || deck->discard_pile_count == 0) {
        return;
    }
    
    // Move all discard cards to draw pile
    for (int i = 0; i < deck->discard_pile_count; i++) {
        deck->draw_pile[deck->draw_pile_count] = deck->discard_pile[i];
        deck->draw_pile_count++;
    }
    
    // Clear discard pile
    deck->discard_pile_count = 0;
    
    // Shuffle the draw pile
    shuffle_deck(deck);
}

// Get total number of cards in deck (draw + discard + hand)
int get_deck_total_count(const Deck* deck) {
    if (!deck) {
        return 0;
    }
    return deck->draw_pile_count + deck->discard_pile_count + deck->hand_count;
}

// Get number of cards in draw pile
int get_draw_pile_count(const Deck* deck) {
    if (!deck) {
        return 0;
    }
    return deck->draw_pile_count;
}

// Get number of cards in discard pile
int get_discard_pile_count(const Deck* deck) {
    if (!deck) {
        return 0;
    }
    return deck->discard_pile_count;
}

// Get number of cards in hand
int get_hand_count(const Deck* deck) {
    if (!deck) {
        return 0;
    }
    return deck->hand_count;
}

// Create a starter deck with basic cards
void create_starter_deck(Deck* deck, const CardPool* pool) {
    if (!deck || !pool) {
        return;
    }
    
    init_deck(deck);
    
    // Add 5 Strikes
    Card* strike = get_card_by_id(pool, CARD_STRIKE);
    for (int i = 0; i < 5; i++) {
        add_card_to_deck(deck, strike);
    }
    
    // Add 4 Defends
    Card* defend = get_card_by_id(pool, CARD_DEFEND);
    for (int i = 0; i < 4; i++) {
        add_card_to_deck(deck, defend);
    }
    
    // Add 1 Bash
    Card* bash = get_card_by_id(pool, CARD_BASH);
    if (bash) {
        add_card_to_deck(deck, bash);
    }
    
    // Shuffle the deck
    shuffle_deck(deck);
}
