#include "player.h"
#include <stdio.h>

// Initialize a player with default values
void init_player(Player* player) {
    if (!player) {
        return;
    }
    
    player->max_hp = 100;
    player->hp = 100;
    player->max_energy = 3;
    player->energy = 3;
    player->block = 0;
    player->strength = 0;
    player->dexterity = 0;
    player->is_dead = false;
}

// Player takes damage, considering block
void take_damage(Player* player, int damage) {
    if (!player || damage < 0) {
        return;
    }
    
    // Apply block first
    if (player->block > 0) {
        if (damage <= player->block) {
            // All damage absorbed by block
            player->block -= damage;
            return;
        } else {
            // Block partially absorbs damage
            damage -= player->block;
            player->block = 0;
        }
    }
    
    // Apply remaining damage to HP
    player->hp -= damage;
    if (player->hp < 0) {
        player->hp = 0;
    }
    
    // Check for death
    if (player->hp <= 0) {
        player->is_dead = true;
    }
}

// Heal the player
void heal_player(Player* player, int heal_amount) {
    if (!player || heal_amount < 0) {
        return;
    }
    
    player->hp += heal_amount;
    if (player->hp > player->max_hp) {
        player->hp = player->max_hp;
    }
    
    // If player was dead and now has HP, revive them
    if (player->is_dead && player->hp > 0) {
        player->is_dead = false;
    }
}

// Spend energy if available
bool spend_energy(Player* player, int amount) {
    if (!player || amount < 0) {
        return false;
    }
    
    if (player->energy >= amount) {
        player->energy -= amount;
        return true;
    }
    
    return false;
}

// Reset energy to maximum
void reset_energy(Player* player) {
    if (!player) {
        return;
    }
    
    player->energy = player->max_energy;
}

// Add block to the player
void add_block(Player* player, int block_amount) {
    if (!player || block_amount < 0) {
        return;
    }
    
    player->block += block_amount;
}

// Add strength to the player
void add_strength(Player* player, int strength_amount) {
    if (!player || strength_amount < 0) {
        return;
    }
    
    player->strength += strength_amount;
}

// Add dexterity to the player
void add_dexterity(Player* player, int dexterity_amount) {
    if (!player || dexterity_amount < 0) {
        return;
    }
    
    player->dexterity += dexterity_amount;
}

// Validate player state
bool is_valid_player(const Player* player) {
    if (!player) {
        return false;
    }
    
    // Check basic validity
    if (player->max_hp <= 0 || player->hp < 0 || player->hp > player->max_hp) {
        return false;
    }
    
    if (player->max_energy <= 0 || player->energy < 0 || player->energy > player->max_energy) {
        return false;
    }
    
    if (player->block < 0 || player->strength < 0 || player->dexterity < 0) {
        return false;
    }
    
    return true;
}

// End player turn (clear block, reset energy, keep status effects)
void end_player_turn(Player* player) {
    if (!player) {
        return;
    }
    
    // Clear block
    player->block = 0;
    
    // Reset energy
    reset_energy(player);
    
    // Status effects (strength, dexterity) persist until explicitly removed
}

