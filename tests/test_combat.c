#include "unity.h"
#include "combat.h"
#include "player.h"
#include "enemy.h"
#include "cards.h"
#include "deck.h"
#include <stdio.h>

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Test combat initialization
void test_combat_init(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    init_combat(&combat, &player, &enemy);
    
    TEST_ASSERT_EQUAL(COMBAT_STATE_PLAYER_TURN, combat.state);
    TEST_ASSERT_EQUAL(0, combat.turn_number);
    TEST_ASSERT_FALSE(combat.is_finished);
    TEST_ASSERT_NULL(combat.winner);
}

// Test player playing a card
void test_player_play_card(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    CardPool pool;
    Deck deck;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    init_card_pool(&pool);
    init_deck(&deck);
    
    init_combat(&combat, &player, &enemy);
    
    // Add a Strike card to hand
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    deck.hand[0] = *strike;
    deck.hand_count = 1;
    
    // Play the card
    bool success = play_card(&combat, &deck, 0);
    
    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(2, player.energy); // Should have spent 1 energy
    TEST_ASSERT_EQUAL(44, enemy.hp); // Should have taken 6 damage
    TEST_ASSERT_EQUAL(0, deck.hand_count); // Card should be discarded
}

// Test player playing card without enough energy
void test_player_play_card_no_energy(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    CardPool pool;
    Deck deck;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    init_card_pool(&pool);
    init_deck(&deck);
    
    init_combat(&combat, &player, &enemy);
    
    // Spend all energy
    spend_energy(&player, 3);
    
    // Add a Strike card to hand
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    deck.hand[0] = *strike;
    deck.hand_count = 1;
    
    // Try to play the card
    bool success = play_card(&combat, &deck, 0);
    
    TEST_ASSERT_FALSE(success);
    TEST_ASSERT_EQUAL(0, player.energy); // Energy should remain 0
    TEST_ASSERT_EQUAL(50, enemy.hp); // Enemy should be unharmed
    TEST_ASSERT_EQUAL(1, deck.hand_count); // Card should remain in hand
}

// Test player playing defend card
void test_player_play_defend(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    CardPool pool;
    Deck deck;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    init_card_pool(&pool);
    init_deck(&deck);
    
    init_combat(&combat, &player, &enemy);
    
    // Add a Defend card to hand
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    deck.hand[0] = *defend;
    deck.hand_count = 1;
    
    // Play the card
    bool success = play_card(&combat, &deck, 0);
    
    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(2, player.energy); // Should have spent 1 energy
    TEST_ASSERT_EQUAL(5, player.block); // Should have gained 5 block
    TEST_ASSERT_EQUAL(50, enemy.hp); // Enemy should be unharmed
}

// Test enemy turn
void test_enemy_turn(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    init_combat(&combat, &player, &enemy);
    
    // Generate enemy intent
    generate_enemy_intent(&enemy);
    
    // Execute enemy turn
    execute_enemy_turn(&combat);
    
    // Enemy should have performed an action
    TEST_ASSERT_TRUE(enemy.intent != ENEMY_INTENT_NONE);
    
    // If enemy attacked, player should have taken damage or block
    if (enemy.intent == ENEMY_INTENT_ATTACK) {
        TEST_ASSERT_TRUE(player.hp < 100 || player.block > 0);
    }
}

// Test combat turn progression
void test_combat_turn_progression(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    init_combat(&combat, &player, &enemy);
    
    TEST_ASSERT_EQUAL(COMBAT_STATE_PLAYER_TURN, combat.state);
    TEST_ASSERT_EQUAL(0, combat.turn_number);
    
    // End player turn
    end_player_combat_turn(&combat);
    
    TEST_ASSERT_EQUAL(COMBAT_STATE_ENEMY_TURN, combat.state);
    TEST_ASSERT_EQUAL(0, combat.turn_number);
    
    // Execute enemy turn
    execute_enemy_turn(&combat);
    
    TEST_ASSERT_EQUAL(COMBAT_STATE_PLAYER_TURN, combat.state);
    TEST_ASSERT_EQUAL(1, combat.turn_number);
}

// Test combat end conditions
void test_combat_end_player_death(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    init_combat(&combat, &player, &enemy);
    
    // Kill the player
    take_damage(&player, 150);
    
    // Check combat end
    check_combat_end(&combat);
    
    TEST_ASSERT_TRUE(combat.is_finished);
    TEST_ASSERT_EQUAL(COMBAT_RESULT_PLAYER_DEFEATED, combat.result);
    TEST_ASSERT_EQUAL(&enemy, combat.winner);
}

// Test combat end enemy death
void test_combat_end_enemy_death(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    init_combat(&combat, &player, &enemy);
    
    // Kill the enemy
    take_damage_enemy(&enemy, 100);
    
    // Check combat end
    check_combat_end(&combat);
    
    TEST_ASSERT_TRUE(combat.is_finished);
    TEST_ASSERT_EQUAL(COMBAT_RESULT_PLAYER_VICTORY, combat.result);
    TEST_ASSERT_EQUAL(&player, combat.winner);
}

// Test damage calculation with strength
void test_damage_calculation_with_strength(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    CardPool pool;
    Deck deck;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    init_card_pool(&pool);
    init_deck(&deck);
    
    init_combat(&combat, &player, &enemy);
    
    // Add strength to player
    add_strength(&player, 3);
    
    // Add a Strike card to hand
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    deck.hand[0] = *strike;
    deck.hand_count = 1;
    
    // Play the card
    bool success = play_card(&combat, &deck, 0);
    
    TEST_ASSERT_TRUE(success);
    TEST_ASSERT_EQUAL(41, enemy.hp); // Should have taken 6 + 3 = 9 damage
}

// Test block mechanics in combat
void test_combat_block_mechanics(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    CardPool pool;
    Deck deck;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    init_card_pool(&pool);
    init_deck(&deck);
    
    init_combat(&combat, &player, &enemy);
    
    // Add block to player
    add_block(&player, 10);
    
    // Generate enemy attack intent
    enemy.intent = ENEMY_INTENT_ATTACK;
    
    // Execute enemy turn
    execute_enemy_turn(&combat);
    
    // Player should have taken reduced damage
    TEST_ASSERT_TRUE(player.hp >= 90); // Should have taken at most 10 damage
}

// Test combat validation
void test_combat_validation(void) {
    Combat combat;
    Player player;
    Enemy enemy;
    
    init_player(&player);
    init_enemy(&enemy, ENEMY_CULTIST, "Cultist");
    
    init_combat(&combat, &player, &enemy);
    
    TEST_ASSERT_TRUE(is_valid_combat(&combat));
    
    // Corrupt combat data
    combat.state = 999; // Invalid state
    TEST_ASSERT_FALSE(is_valid_combat(&combat));
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_combat_init);
    RUN_TEST(test_player_play_card);
    RUN_TEST(test_player_play_card_no_energy);
    RUN_TEST(test_player_play_defend);
    RUN_TEST(test_enemy_turn);
    RUN_TEST(test_combat_turn_progression);
    RUN_TEST(test_combat_end_player_death);
    RUN_TEST(test_combat_end_enemy_death);
    RUN_TEST(test_damage_calculation_with_strength);
    RUN_TEST(test_combat_block_mechanics);
    RUN_TEST(test_combat_validation);
    
    return UNITY_END();
}
