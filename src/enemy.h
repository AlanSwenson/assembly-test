#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>

// Enemy types
typedef enum {
    ENEMY_CULTIST = 1,
    ENEMY_GOBLIN,
    ENEMY_ORC,
    ENEMY_SKELETON,
    ENEMY_DRAGON
} EnemyType;

// Enemy intents
typedef enum {
    ENEMY_INTENT_NONE = 0,
    ENEMY_INTENT_ATTACK,
    ENEMY_INTENT_DEFEND,
    ENEMY_INTENT_BUFF
} EnemyIntent;

// Enemy actions
typedef enum {
    ENEMY_ACTION_NONE = 0,
    ENEMY_ACTION_ATTACK,
    ENEMY_ACTION_DEFEND,
    ENEMY_ACTION_BUFF
} EnemyActionType;

// Enemy action structure
typedef struct {
    EnemyActionType type;
    int damage;
    int block;
    int strength;
} EnemyAction;

// Enemy structure
typedef struct {
    EnemyType type;
    char name[32];
    int max_hp;
    int hp;
    int block;
    int strength;
    EnemyIntent intent;
    bool is_dead;
} Enemy;

// Enemy management functions
void init_enemy(Enemy* enemy, EnemyType type, const char* name);

void take_damage_enemy(Enemy* enemy, int damage);

void add_block_enemy(Enemy* enemy, int block_amount);

void add_strength_enemy(Enemy* enemy, int strength_amount);

void generate_enemy_intent(Enemy* enemy);

EnemyAction execute_enemy_action(Enemy* enemy);

bool is_valid_enemy(const Enemy* enemy);

void end_enemy_turn(Enemy* enemy);

#endif // ENEMY_H

