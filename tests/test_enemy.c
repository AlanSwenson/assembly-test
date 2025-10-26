#include "unity.h"
#include "enemy.h"
#include <stdio.h>

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Test enemy creation and initialization
void test_enemy_creation(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    TEST_ASSERT_EQUAL(ENEMY_CULTIST, enemy.type);
    TEST_ASSERT_EQUAL_STRING("Cultist", enemy.name);
    TEST_ASSERT_EQUAL(50, enemy.max_hp);
    TEST_ASSERT_EQUAL(50, enemy.hp);
    TEST_ASSERT_EQUAL(0, enemy.block);
    TEST_ASSERT_EQUAL(0, enemy.strength);
    TEST_ASSERT_FALSE(enemy.is_dead);
    TEST_ASSERT_EQUAL(ENEMY_INTENT_NONE, enemy.intent);
}

// Test different enemy types
void test_enemy_types(void) {
    Enemy cultist, goblin, orc;
    
    init_enemy(&cultist, ENEMY_CULTIST, "Cultist");
    init_enemy(&goblin, ENEMY_GOBLIN, "Goblin");
    init_enemy(&orc, ENEMY_ORC, "Orc");
    
    // Test Cultist stats
    TEST_ASSERT_EQUAL(50, cultist.max_hp);
    TEST_ASSERT_EQUAL(50, cultist.hp);
    
    // Test Goblin stats
    TEST_ASSERT_EQUAL(30, goblin.max_hp);
    TEST_ASSERT_EQUAL(30, goblin.hp);
    
    // Test Orc stats
    TEST_ASSERT_EQUAL(80, orc.max_hp);
    TEST_ASSERT_EQUAL(80, orc.hp);
}

// Test enemy taking damage
void test_enemy_take_damage(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    // Take 20 damage
    take_damage_enemy(&enemy, 20);
    TEST_ASSERT_EQUAL(30, enemy.hp);
    TEST_ASSERT_FALSE(enemy.is_dead);
    
    // Take more damage
    take_damage_enemy(&enemy, 30);
    TEST_ASSERT_EQUAL(0, enemy.hp);
    TEST_ASSERT_TRUE(enemy.is_dead);
}

// Test enemy block mechanics
void test_enemy_block(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    // Add block
    add_block_enemy(&enemy, 15);
    TEST_ASSERT_EQUAL(15, enemy.block);
    
    // Take damage - should reduce block first
    take_damage_enemy(&enemy, 10);
    TEST_ASSERT_EQUAL(5, enemy.block);
    TEST_ASSERT_EQUAL(50, enemy.hp); // HP should be unchanged
    
    // Take more damage - should consume remaining block and damage HP
    take_damage_enemy(&enemy, 10);
    TEST_ASSERT_EQUAL(0, enemy.block);
    TEST_ASSERT_EQUAL(45, enemy.hp);
}

// Test enemy intent generation
void test_enemy_intent_generation(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    // Generate intent
    generate_enemy_intent(&enemy);
    
    // Intent should be one of the valid types
    TEST_ASSERT_TRUE(enemy.intent == ENEMY_INTENT_ATTACK || 
                     enemy.intent == ENEMY_INTENT_DEFEND ||
                     enemy.intent == ENEMY_INTENT_BUFF);
}

// Test enemy AI patterns
void test_enemy_ai_patterns(void) {
    Enemy cultist, goblin, orc;
    
    init_enemy(&cultist, ENEMY_CULTIST, "Cultist");
    init_enemy(&goblin, ENEMY_GOBLIN, "Goblin");
    init_enemy(&orc, ENEMY_ORC, "Orc");
    
    // Test multiple intent generations to see patterns
    int attack_count = 0, defend_count = 0, buff_count = 0;
    
    for (int i = 0; i < 100; i++) {
        generate_enemy_intent(&cultist);
        switch (cultist.intent) {
            case ENEMY_INTENT_ATTACK: attack_count++; break;
            case ENEMY_INTENT_DEFEND: defend_count++; break;
            case ENEMY_INTENT_BUFF: buff_count++; break;
        }
    }
    
    // Should have some variety in intents
    TEST_ASSERT_TRUE(attack_count > 0);
    TEST_ASSERT_TRUE(defend_count > 0);
    TEST_ASSERT_TRUE(buff_count > 0);
}

// Test enemy action execution
void test_enemy_action_execution(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    // Set attack intent
    enemy.intent = ENEMY_INTENT_ATTACK;
    EnemyAction action = execute_enemy_action(&enemy);
    
    TEST_ASSERT_EQUAL(ENEMY_ACTION_ATTACK, action.type);
    TEST_ASSERT_TRUE(action.damage > 0);
    
    // Set defend intent
    enemy.intent = ENEMY_INTENT_DEFEND;
    action = execute_enemy_action(&enemy);
    
    TEST_ASSERT_EQUAL(ENEMY_ACTION_DEFEND, action.type);
    TEST_ASSERT_TRUE(action.block > 0);
    
    // Set buff intent
    enemy.intent = ENEMY_INTENT_BUFF;
    action = execute_enemy_action(&enemy);
    
    TEST_ASSERT_EQUAL(ENEMY_ACTION_BUFF, action.type);
    TEST_ASSERT_TRUE(action.strength > 0);
}

// Test enemy validation
void test_enemy_validation(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    TEST_ASSERT_TRUE(is_valid_enemy(&enemy));
    
    // Corrupt enemy data
    enemy.hp = -10;
    TEST_ASSERT_FALSE(is_valid_enemy(&enemy));
    
    // Reset and test other invalid states
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    enemy.max_hp = 0;
    TEST_ASSERT_FALSE(is_valid_enemy(&enemy));
}

// Test enemy turn end
void test_enemy_turn_end(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    // Add some block and status effects
    add_block_enemy(&enemy, 10);
    add_strength_enemy(&enemy, 2);
    
    // End turn
    end_enemy_turn(&enemy);
    
    // Block should be cleared, status effects remain
    TEST_ASSERT_EQUAL(0, enemy.block);
    TEST_ASSERT_EQUAL(2, enemy.strength);
}

// Test enemy strength addition
void test_enemy_strength(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    add_strength_enemy(&enemy, 3);
    TEST_ASSERT_EQUAL(3, enemy.strength);
    
    add_strength_enemy(&enemy, 2);
    TEST_ASSERT_EQUAL(5, enemy.strength);
}

// Test enemy damage calculation with strength
void test_enemy_damage_with_strength(void) {
    Enemy enemy;
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    // Add strength
    add_strength_enemy(&enemy, 3);
    
    // Set attack intent and execute
    enemy.intent = ENEMY_INTENT_ATTACK;
    EnemyAction action = execute_enemy_action(&enemy);
    
    // Damage should be increased by strength
    TEST_ASSERT_TRUE(action.damage > 6); // Base damage + strength
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_enemy_creation);
    RUN_TEST(test_enemy_types);
    RUN_TEST(test_enemy_take_damage);
    RUN_TEST(test_enemy_block);
    RUN_TEST(test_enemy_intent_generation);
    RUN_TEST(test_enemy_ai_patterns);
    RUN_TEST(test_enemy_action_execution);
    RUN_TEST(test_enemy_validation);
    RUN_TEST(test_enemy_turn_end);
    RUN_TEST(test_enemy_strength);
    RUN_TEST(test_enemy_damage_with_strength);
    
    return UNITY_END();
}

