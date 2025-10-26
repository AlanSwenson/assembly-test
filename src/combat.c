#include "combat.h"
#include <stdio.h>

// Initialize combat between player and enemy
void init_combat(Combat* combat, Player* player, Enemy* enemy) {
    if (!combat || !player || !enemy) {
        return;
    }
    
    combat->player = player;
    combat->enemy = enemy;
    combat->state = COMBAT_STATE_PLAYER_TURN;
    combat->result = COMBAT_RESULT_ONGOING;
    combat->turn_number = 0;
    combat->is_finished = false;
    combat->winner = NULL;
}

// Player plays a card from their hand
bool play_card(Combat* combat, Deck* deck, int hand_index) {
    if (!combat || !deck || hand_index < 0 || hand_index >= deck->hand_count) {
        return false;
    }
    
    if (combat->state != COMBAT_STATE_PLAYER_TURN) {
        return false;
    }
    
    Card* card = &deck->hand[hand_index];
    Player* player = combat->player;
    
    // Check if player has enough energy
    if (!spend_energy(player, card->cost)) {
        return false;
    }
    
    // Apply card effects
    if (card->damage > 0) {
        // Deal damage to enemy
        int total_damage = card->damage + player->strength;
        take_damage_enemy(combat->enemy, total_damage);
    }
    
    if (card->block > 0) {
        // Add block to player
        add_block(player, card->block + player->dexterity);
    }
    
    if (card->heal > 0) {
        // Heal player
        heal_player(player, card->heal);
    }
    
    // Discard the card
    discard_card(deck, hand_index);
    
    return true;
}

// Execute enemy turn
void execute_enemy_turn(Combat* combat) {
    if (!combat || combat->state != COMBAT_STATE_ENEMY_TURN) {
        return;
    }
    
    Enemy* enemy = combat->enemy;
    Player* player = combat->player;
    
    // Generate enemy intent if not already set
    if (enemy->intent == ENEMY_INTENT_NONE) {
        generate_enemy_intent(enemy);
    }
    
    // Execute enemy action
    EnemyAction action = execute_enemy_action(enemy);
    
    switch (action.type) {
        case ENEMY_ACTION_ATTACK:
            // Enemy attacks player
            take_damage(player, action.damage);
            break;
            
        case ENEMY_ACTION_DEFEND:
            // Enemy gains block
            add_block_enemy(enemy, action.block);
            break;
            
        case ENEMY_ACTION_BUFF:
            // Enemy gains strength
            add_strength_enemy(enemy, action.strength);
            break;
            
        default:
            break;
    }
    
    // End enemy turn
    end_enemy_turn(enemy);
    
    // Switch to player turn
    combat->state = COMBAT_STATE_PLAYER_TURN;
    combat->turn_number++;
    
    // Reset player energy for new turn
    reset_energy(player);
}

// End player turn
void end_player_combat_turn(Combat* combat) {
    if (!combat || combat->state != COMBAT_STATE_PLAYER_TURN) {
        return;
    }
    
    Player* player = combat->player;
    
    // End player turn (clear block, reset energy)
    end_player_turn(player);
    
    // Switch to enemy turn
    combat->state = COMBAT_STATE_ENEMY_TURN;
}

// Check if combat has ended
void check_combat_end(Combat* combat) {
    if (!combat || combat->is_finished) {
        return;
    }
    
    Player* player = combat->player;
    Enemy* enemy = combat->enemy;
    
    if (player->is_dead) {
        combat->is_finished = true;
        combat->result = COMBAT_RESULT_PLAYER_DEFEATED;
        combat->winner = enemy;
        combat->state = COMBAT_STATE_FINISHED;
    } else if (enemy->is_dead) {
        combat->is_finished = true;
        combat->result = COMBAT_RESULT_PLAYER_VICTORY;
        combat->winner = player;
        combat->state = COMBAT_STATE_FINISHED;
    }
}

// Validate combat state
bool is_valid_combat(const Combat* combat) {
    if (!combat || !combat->player || !combat->enemy) {
        return false;
    }
    
    // Check state is valid
    if (combat->state < COMBAT_STATE_PLAYER_TURN || 
        combat->state > COMBAT_STATE_FINISHED) {
        return false;
    }
    
    // Check turn number is non-negative
    if (combat->turn_number < 0) {
        return false;
    }
    
    // Check result is valid
    if (combat->result < COMBAT_RESULT_ONGOING || 
        combat->result > COMBAT_RESULT_PLAYER_DEFEATED) {
        return false;
    }
    
    // If combat is finished, winner should be set
    if (combat->is_finished && !combat->winner) {
        return false;
    }
    
    // If combat is finished, result should not be ongoing
    if (combat->is_finished && combat->result == COMBAT_RESULT_ONGOING) {
        return false;
    }
    
    return true;
}
