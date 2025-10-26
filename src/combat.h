#ifndef COMBAT_H
#define COMBAT_H

#include "player.h"
#include "enemy.h"
#include "cards.h"
#include "deck.h"
#include <stdbool.h>

// Combat states
typedef enum {
    COMBAT_STATE_PLAYER_TURN = 1,
    COMBAT_STATE_ENEMY_TURN,
    COMBAT_STATE_FINISHED
} CombatState;

// Combat results
typedef enum {
    COMBAT_RESULT_ONGOING = 0,
    COMBAT_RESULT_PLAYER_VICTORY,
    COMBAT_RESULT_PLAYER_DEFEATED
} CombatResult;

// Combat structure
typedef struct {
    Player* player;
    Enemy* enemy;
    CombatState state;
    CombatResult result;
    int turn_number;
    bool is_finished;
    void* winner; // Points to player or enemy
} Combat;

// Combat management functions
void init_combat(Combat* combat, Player* player, Enemy* enemy);

bool play_card(Combat* combat, Deck* deck, int hand_index);

void execute_enemy_turn(Combat* combat);

void end_player_combat_turn(Combat* combat);

void check_combat_end(Combat* combat);

bool is_valid_combat(const Combat* combat);

#endif // COMBAT_H
