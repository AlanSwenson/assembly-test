#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

// Player structure
typedef struct {
    int max_hp;
    int hp;
    int max_energy;
    int energy;
    int block;
    int strength;
    int dexterity;
    bool is_dead;
} Player;

// Player management functions
void init_player(Player* player);

void take_damage(Player* player, int damage);

void heal_player(Player* player, int heal_amount);

bool spend_energy(Player* player, int amount);

void reset_energy(Player* player);

void add_block(Player* player, int block_amount);

void add_strength(Player* player, int strength_amount);

void add_dexterity(Player* player, int dexterity_amount);

bool is_valid_player(const Player* player);

void end_player_turn(Player* player);

#endif // PLAYER_H

