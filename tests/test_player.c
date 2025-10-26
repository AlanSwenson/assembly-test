#include "unity.h"
#include "player.h"
#include <stdio.h>

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Test player creation and initialization
void test_player_creation(void) {
    Player player;
    init_player(&player);
    
    TEST_ASSERT_EQUAL(100, player.max_hp);
    TEST_ASSERT_EQUAL(100, player.hp);
    TEST_ASSERT_EQUAL(3, player.max_energy);
    TEST_ASSERT_EQUAL(3, player.energy);
    TEST_ASSERT_EQUAL(0, player.block);
    TEST_ASSERT_EQUAL(0, player.strength);
    TEST_ASSERT_EQUAL(0, player.dexterity);
    TEST_ASSERT_FALSE(player.is_dead);
}

// Test player taking damage
void test_player_take_damage(void) {
    Player player;
    init_player(&player);
    
    // Take 20 damage
    take_damage(&player, 20);
    TEST_ASSERT_EQUAL(80, player.hp);
    TEST_ASSERT_FALSE(player.is_dead);
    
    // Take more damage
    take_damage(&player, 30);
    TEST_ASSERT_EQUAL(50, player.hp);
    TEST_ASSERT_FALSE(player.is_dead);
}

// Test player death
void test_player_death(void) {
    Player player;
    init_player(&player);
    
    // Take lethal damage
    take_damage(&player, 150);
    TEST_ASSERT_EQUAL(0, player.hp);
    TEST_ASSERT_TRUE(player.is_dead);
}

// Test player healing
void test_player_heal(void) {
    Player player;
    init_player(&player);
    
    // Take some damage first
    take_damage(&player, 50);
    TEST_ASSERT_EQUAL(50, player.hp);
    
    // Heal
    heal_player(&player, 30);
    TEST_ASSERT_EQUAL(80, player.hp);
    TEST_ASSERT_FALSE(player.is_dead);
}

// Test healing beyond max HP
void test_player_heal_beyond_max(void) {
    Player player;
    init_player(&player);
    
    // Take some damage
    take_damage(&player, 20);
    TEST_ASSERT_EQUAL(80, player.hp);
    
    // Heal more than needed
    heal_player(&player, 50);
    TEST_ASSERT_EQUAL(100, player.hp); // Should cap at max_hp
}

// Test energy management
void test_player_energy(void) {
    Player player;
    init_player(&player);
    
    TEST_ASSERT_EQUAL(3, player.energy);
    
    // Spend energy
    TEST_ASSERT_TRUE(spend_energy(&player, 2));
    TEST_ASSERT_EQUAL(1, player.energy);
    
    // Try to spend more than available
    TEST_ASSERT_FALSE(spend_energy(&player, 3));
    TEST_ASSERT_EQUAL(1, player.energy); // Should remain unchanged
    
    // Spend remaining energy
    TEST_ASSERT_TRUE(spend_energy(&player, 1));
    TEST_ASSERT_EQUAL(0, player.energy);
}

// Test energy reset
void test_player_energy_reset(void) {
    Player player;
    init_player(&player);
    
    // Spend all energy
    spend_energy(&player, 3);
    TEST_ASSERT_EQUAL(0, player.energy);
    
    // Reset energy
    reset_energy(&player);
    TEST_ASSERT_EQUAL(3, player.energy);
}

// Test block mechanics
void test_player_block(void) {
    Player player;
    init_player(&player);
    
    // Add block
    add_block(&player, 15);
    TEST_ASSERT_EQUAL(15, player.block);
    
    // Take damage - should reduce block first
    take_damage(&player, 10);
    TEST_ASSERT_EQUAL(5, player.block);
    TEST_ASSERT_EQUAL(100, player.hp); // HP should be unchanged
    
    // Take more damage - should consume remaining block and damage HP
    take_damage(&player, 10);
    TEST_ASSERT_EQUAL(0, player.block);
    TEST_ASSERT_EQUAL(95, player.hp);
}

// Test status effects
void test_player_status_effects(void) {
    Player player;
    init_player(&player);
    
    // Add strength
    add_strength(&player, 3);
    TEST_ASSERT_EQUAL(3, player.strength);
    
    // Add dexterity
    add_dexterity(&player, 2);
    TEST_ASSERT_EQUAL(2, player.dexterity);
    
    // Add more strength
    add_strength(&player, 1);
    TEST_ASSERT_EQUAL(4, player.strength);
}

// Test player validation
void test_player_validation(void) {
    Player player;
    init_player(&player);
    
    TEST_ASSERT_TRUE(is_valid_player(&player));
    
    // Corrupt player data
    player.hp = -10;
    TEST_ASSERT_FALSE(is_valid_player(&player));
    
    // Reset and test other invalid states
    init_player(&player);
    player.max_hp = 0;
    TEST_ASSERT_FALSE(is_valid_player(&player));
}

// Test player turn end
void test_player_turn_end(void) {
    Player player;
    init_player(&player);
    
    // Add some block and status effects
    add_block(&player, 10);
    add_strength(&player, 2);
    add_dexterity(&player, 1);
    
    // End turn
    end_player_turn(&player);
    
    // Block should be cleared, energy reset, status effects remain
    TEST_ASSERT_EQUAL(0, player.block);
    TEST_ASSERT_EQUAL(3, player.energy);
    TEST_ASSERT_EQUAL(2, player.strength);
    TEST_ASSERT_EQUAL(1, player.dexterity);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_player_creation);
    RUN_TEST(test_player_take_damage);
    RUN_TEST(test_player_death);
    RUN_TEST(test_player_heal);
    RUN_TEST(test_player_heal_beyond_max);
    RUN_TEST(test_player_energy);
    RUN_TEST(test_player_energy_reset);
    RUN_TEST(test_player_block);
    RUN_TEST(test_player_status_effects);
    RUN_TEST(test_player_validation);
    RUN_TEST(test_player_turn_end);
    
    return UNITY_END();
}

