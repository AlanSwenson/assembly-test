#ifndef CARDS_H
#define CARDS_H

#include <stdint.h>
#include <stdbool.h>

// Maximum number of cards in the card pool
#define MAX_CARDS 50

// Card IDs
typedef enum {
    CARD_STRIKE = 1,
    CARD_DEFEND,
    CARD_HEAL,
    CARD_BASH,
    CARD_SLASH,
    CARD_BLOCK,
    CARD_POWER_STRIKE,
    CARD_POWER_DEFEND,
    CARD_DOUBLE_STRIKE,
    CARD_HEAVY_BLOCK,
    CARD_VAMPIRE_STRIKE,
    CARD_POISON_STRIKE,
    CARD_FIREBALL,
    CARD_ICE_SHARD,
    CARD_LIGHTNING_BOLT,
    CARD_HEALING_POTION,
    CARD_ENERGY_POTION,
    CARD_SHIELD_BASH,
    CARD_COUNTER_ATTACK,
    CARD_BERSERKER_RAGE
} CardID;

// Card types
typedef enum {
    CARD_TYPE_ATTACK = 1,
    CARD_TYPE_SKILL,
    CARD_TYPE_POWER
} CardType;

// Card structure
typedef struct {
    CardID id;
    char name[32];
    int cost;
    CardType type;
    int damage;
    int block;
    int heal;
} Card;

// Card pool structure
typedef struct {
    Card cards[MAX_CARDS];
    int count;
} CardPool;

// Card functions
void init_card(Card* card, CardID id, const char* name, int cost, CardType type, 
               int damage, int block, int heal);

bool cards_equal(const Card* card1, const Card* card2);

bool is_valid_card(const Card* card);

void init_card_pool(CardPool* pool);

Card* get_card_by_id(const CardPool* pool, CardID id);

#endif // CARDS_H
