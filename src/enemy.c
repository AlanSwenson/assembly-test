#include "enemy.h"
#include "random.h"
#include <string.h>
#include <stdio.h>

// Initialize an enemy with given type and name
void init_enemy(Enemy* enemy, EnemyType type, const char* name) {
    if (!enemy || !name) {
        return;
    }
    
    enemy->type = type;
    strncpy(enemy->name, name, sizeof(enemy->name) - 1);
    enemy->name[sizeof(enemy->name) - 1] = '\0';
    
    // Set stats based on enemy type
    switch (type) {
        case ENEMY_CULTIST:
            enemy->max_hp = 50;
            enemy->hp = 50;
            break;
        case ENEMY_GOBLIN:
            enemy->max_hp = 30;
            enemy->hp = 30;
            break;
        case ENEMY_ORC:
            enemy->max_hp = 80;
            enemy->hp = 80;
            break;
        case ENEMY_SKELETON:
            enemy->max_hp = 40;
            enemy->hp = 40;
            break;
        case ENEMY_DRAGON:
            enemy->max_hp = 200;
            enemy->hp = 200;
            break;
        default:
            enemy->max_hp = 50;
            enemy->hp = 50;
            break;
    }
    
    enemy->block = 0;
    enemy->strength = 0;
    enemy->intent = ENEMY_INTENT_NONE;
    enemy->is_dead = false;
}

// Enemy takes damage, considering block
void take_damage_enemy(Enemy* enemy, int damage) {
    if (!enemy || damage < 0) {
        return;
    }
    
    // Apply block first
    if (enemy->block > 0) {
        if (damage <= enemy->block) {
            // All damage absorbed by block
            enemy->block -= damage;
            return;
        } else {
            // Block partially absorbs damage
            damage -= enemy->block;
            enemy->block = 0;
        }
    }
    
    // Apply remaining damage to HP
    enemy->hp -= damage;
    if (enemy->hp < 0) {
        enemy->hp = 0;
    }
    
    // Check for death
    if (enemy->hp <= 0) {
        enemy->is_dead = true;
    }
}

// Add block to the enemy
void add_block_enemy(Enemy* enemy, int block_amount) {
    if (!enemy || block_amount < 0) {
        return;
    }
    
    enemy->block += block_amount;
}

// Add strength to the enemy
void add_strength_enemy(Enemy* enemy, int strength_amount) {
    if (!enemy || strength_amount < 0) {
        return;
    }
    
    enemy->strength += strength_amount;
}

// Generate enemy intent based on AI patterns
void generate_enemy_intent(Enemy* enemy) {
    if (!enemy) {
        return;
    }
    
    // Simple AI patterns based on enemy type
    uint64_t roll = random_range(100);
    
    switch (enemy->type) {
        case ENEMY_CULTIST:
            // Cultist: 60% attack, 25% defend, 15% buff
            if (roll < 60) {
                enemy->intent = ENEMY_INTENT_ATTACK;
            } else if (roll < 85) {
                enemy->intent = ENEMY_INTENT_DEFEND;
            } else {
                enemy->intent = ENEMY_INTENT_BUFF;
            }
            break;
            
        case ENEMY_GOBLIN:
            // Goblin: 80% attack, 20% defend
            if (roll < 80) {
                enemy->intent = ENEMY_INTENT_ATTACK;
            } else {
                enemy->intent = ENEMY_INTENT_DEFEND;
            }
            break;
            
        case ENEMY_ORC:
            // Orc: 70% attack, 20% defend, 10% buff
            if (roll < 70) {
                enemy->intent = ENEMY_INTENT_ATTACK;
            } else if (roll < 90) {
                enemy->intent = ENEMY_INTENT_DEFEND;
            } else {
                enemy->intent = ENEMY_INTENT_BUFF;
            }
            break;
            
        case ENEMY_SKELETON:
            // Skeleton: 50% attack, 30% defend, 20% buff
            if (roll < 50) {
                enemy->intent = ENEMY_INTENT_ATTACK;
            } else if (roll < 80) {
                enemy->intent = ENEMY_INTENT_DEFEND;
            } else {
                enemy->intent = ENEMY_INTENT_BUFF;
            }
            break;
            
        case ENEMY_DRAGON:
            // Dragon: 90% attack, 10% defend
            if (roll < 90) {
                enemy->intent = ENEMY_INTENT_ATTACK;
            } else {
                enemy->intent = ENEMY_INTENT_DEFEND;
            }
            break;
            
        default:
            enemy->intent = ENEMY_INTENT_ATTACK;
            break;
    }
}

// Execute enemy action based on intent
EnemyAction execute_enemy_action(Enemy* enemy) {
    EnemyAction action = {ENEMY_ACTION_NONE, 0, 0, 0};
    
    if (!enemy) {
        return action;
    }
    
    switch (enemy->intent) {
        case ENEMY_INTENT_ATTACK:
            action.type = ENEMY_ACTION_ATTACK;
            // Base damage varies by enemy type, plus strength
            switch (enemy->type) {
                case ENEMY_CULTIST:
                    action.damage = 6 + enemy->strength;
                    break;
                case ENEMY_GOBLIN:
                    action.damage = 4 + enemy->strength;
                    break;
                case ENEMY_ORC:
                    action.damage = 8 + enemy->strength;
                    break;
                case ENEMY_SKELETON:
                    action.damage = 5 + enemy->strength;
                    break;
                case ENEMY_DRAGON:
                    action.damage = 20 + enemy->strength;
                    break;
                default:
                    action.damage = 6 + enemy->strength;
                    break;
            }
            break;
            
        case ENEMY_INTENT_DEFEND:
            action.type = ENEMY_ACTION_DEFEND;
            // Block amount varies by enemy type
            switch (enemy->type) {
                case ENEMY_CULTIST:
                    action.block = 8;
                    break;
                case ENEMY_GOBLIN:
                    action.block = 5;
                    break;
                case ENEMY_ORC:
                    action.block = 12;
                    break;
                case ENEMY_SKELETON:
                    action.block = 6;
                    break;
                case ENEMY_DRAGON:
                    action.block = 15;
                    break;
                default:
                    action.block = 8;
                    break;
            }
            break;
            
        case ENEMY_INTENT_BUFF:
            action.type = ENEMY_ACTION_BUFF;
            // Strength gain varies by enemy type
            switch (enemy->type) {
                case ENEMY_CULTIST:
                    action.strength = 2;
                    break;
                case ENEMY_ORC:
                    action.strength = 3;
                    break;
                case ENEMY_SKELETON:
                    action.strength = 1;
                    break;
                default:
                    action.strength = 2;
                    break;
            }
            break;
            
        default:
            action.type = ENEMY_ACTION_NONE;
            break;
    }
    
    return action;
}

// Validate enemy state
bool is_valid_enemy(const Enemy* enemy) {
    if (!enemy) {
        return false;
    }
    
    // Check basic validity
    if (enemy->max_hp <= 0 || enemy->hp < 0 || enemy->hp > enemy->max_hp) {
        return false;
    }
    
    if (enemy->block < 0 || enemy->strength < 0) {
        return false;
    }
    
    // Check name is not empty
    if (strlen(enemy->name) == 0) {
        return false;
    }
    
    return true;
}

// End enemy turn (clear block, keep status effects)
void end_enemy_turn(Enemy* enemy) {
    if (!enemy) {
        return;
    }
    
    // Clear block
    enemy->block = 0;
    
    // Status effects (strength) persist until explicitly removed
}

