#include "cards.h"
#include <string.h>
#include <stdio.h>

// Initialize a card with given parameters
void init_card(Card* card, CardID id, const char* name, int cost, CardType type, 
               int damage, int block, int heal) {
    card->id = id;
    strncpy(card->name, name, sizeof(card->name) - 1);
    card->name[sizeof(card->name) - 1] = '\0'; // Ensure null termination
    card->cost = cost;
    card->type = type;
    card->damage = damage;
    card->block = block;
    card->heal = heal;
}

// Check if two cards are equal
bool cards_equal(const Card* card1, const Card* card2) {
    if (!card1 || !card2) {
        return false;
    }
    
    return (card1->id == card2->id &&
            strcmp(card1->name, card2->name) == 0 &&
            card1->cost == card2->cost &&
            card1->type == card2->type &&
            card1->damage == card2->damage &&
            card1->block == card2->block &&
            card1->heal == card2->heal);
}

// Validate a card
bool is_valid_card(const Card* card) {
    if (!card) {
        return false;
    }
    
    // Check basic validity
    if (card->cost < 0 || card->damage < 0 || card->block < 0 || card->heal < 0) {
        return false;
    }
    
    // Check name is not empty
    if (strlen(card->name) == 0) {
        return false;
    }
    
    // Check type is valid
    if (card->type < CARD_TYPE_ATTACK || card->type > CARD_TYPE_POWER) {
        return false;
    }
    
    return true;
}

// Initialize the card pool with all available cards
void init_card_pool(CardPool* pool) {
    if (!pool) {
        return;
    }
    
    pool->count = 0;
    
    // Basic attack cards
    init_card(&pool->cards[pool->count++], CARD_STRIKE, "Strike", 1, CARD_TYPE_ATTACK, 6, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_BASH, "Bash", 2, CARD_TYPE_ATTACK, 8, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_SLASH, "Slash", 1, CARD_TYPE_ATTACK, 7, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_POWER_STRIKE, "Power Strike", 2, CARD_TYPE_ATTACK, 10, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_DOUBLE_STRIKE, "Double Strike", 1, CARD_TYPE_ATTACK, 5, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_VAMPIRE_STRIKE, "Vampire Strike", 2, CARD_TYPE_ATTACK, 8, 0, 3);
    init_card(&pool->cards[pool->count++], CARD_POISON_STRIKE, "Poison Strike", 1, CARD_TYPE_ATTACK, 4, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_SHIELD_BASH, "Shield Bash", 2, CARD_TYPE_ATTACK, 6, 3, 0);
    init_card(&pool->cards[pool->count++], CARD_COUNTER_ATTACK, "Counter Attack", 1, CARD_TYPE_ATTACK, 9, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_BERSERKER_RAGE, "Berserker Rage", 0, CARD_TYPE_ATTACK, 12, 0, 0);
    
    // Basic skill cards
    init_card(&pool->cards[pool->count++], CARD_DEFEND, "Defend", 1, CARD_TYPE_SKILL, 0, 5, 0);
    init_card(&pool->cards[pool->count++], CARD_BLOCK, "Block", 1, CARD_TYPE_SKILL, 0, 8, 0);
    init_card(&pool->cards[pool->count++], CARD_POWER_DEFEND, "Power Defend", 2, CARD_TYPE_SKILL, 0, 12, 0);
    init_card(&pool->cards[pool->count++], CARD_HEAVY_BLOCK, "Heavy Block", 2, CARD_TYPE_SKILL, 0, 15, 0);
    
    // Healing cards
    init_card(&pool->cards[pool->count++], CARD_HEAL, "Heal", 1, CARD_TYPE_SKILL, 0, 0, 4);
    init_card(&pool->cards[pool->count++], CARD_HEALING_POTION, "Healing Potion", 0, CARD_TYPE_SKILL, 0, 0, 8);
    
    // Energy cards
    init_card(&pool->cards[pool->count++], CARD_ENERGY_POTION, "Energy Potion", 0, CARD_TYPE_SKILL, 0, 0, 0);
    
    // Magic cards
    init_card(&pool->cards[pool->count++], CARD_FIREBALL, "Fireball", 3, CARD_TYPE_ATTACK, 15, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_ICE_SHARD, "Ice Shard", 2, CARD_TYPE_ATTACK, 8, 0, 0);
    init_card(&pool->cards[pool->count++], CARD_LIGHTNING_BOLT, "Lightning Bolt", 2, CARD_TYPE_ATTACK, 12, 0, 0);
}

// Get a card by its ID
Card* get_card_by_id(const CardPool* pool, CardID id) {
    if (!pool) {
        return NULL;
    }
    
    for (int i = 0; i < pool->count; i++) {
        if (pool->cards[i].id == id) {
            return &pool->cards[i];
        }
    }
    
    return NULL;
}
