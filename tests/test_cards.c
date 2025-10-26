#include "unity.h"
#include "cards.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Test card creation and initialization
void test_card_creation(void) {
    Card card;
    init_card(&card, CARD_STRIKE, "Strike", 1, CARD_TYPE_ATTACK, 6, 0, 0);
    
    TEST_ASSERT_EQUAL(CARD_STRIKE, card.id);
    TEST_ASSERT_EQUAL_STRING("Strike", card.name);
    TEST_ASSERT_EQUAL(1, card.cost);
    TEST_ASSERT_EQUAL(CARD_TYPE_ATTACK, card.type);
    TEST_ASSERT_EQUAL(6, card.damage);
    TEST_ASSERT_EQUAL(0, card.block);
    TEST_ASSERT_EQUAL(0, card.heal);
}

// Test card comparison
void test_card_comparison(void) {
    Card card1, card2, card3;
    init_card(&card1, CARD_STRIKE, "Strike", 1, CARD_TYPE_ATTACK, 6, 0, 0);
    init_card(&card2, CARD_STRIKE, "Strike", 1, CARD_TYPE_ATTACK, 6, 0, 0);
    init_card(&card3, CARD_DEFEND, "Defend", 1, CARD_TYPE_SKILL, 0, 5, 0);
    
    TEST_ASSERT_TRUE(cards_equal(&card1, &card2));
    TEST_ASSERT_FALSE(cards_equal(&card1, &card3));
}

// Test card validation
void test_card_validation(void) {
    Card valid_card, invalid_card;
    
    // Valid card
    init_card(&valid_card, CARD_STRIKE, "Strike", 1, CARD_TYPE_ATTACK, 6, 0, 0);
    TEST_ASSERT_TRUE(is_valid_card(&valid_card));
    
    // Invalid card (negative cost)
    init_card(&invalid_card, CARD_STRIKE, "Strike", -1, CARD_TYPE_ATTACK, 6, 0, 0);
    TEST_ASSERT_FALSE(is_valid_card(&invalid_card));
    
    // Invalid card (empty name)
    init_card(&invalid_card, CARD_STRIKE, "", 1, CARD_TYPE_ATTACK, 6, 0, 0);
    TEST_ASSERT_FALSE(is_valid_card(&invalid_card));
}

// Test card pool initialization
void test_card_pool_init(void) {
    CardPool pool;
    init_card_pool(&pool);
    
    TEST_ASSERT_TRUE(pool.count > 0);
    TEST_ASSERT_TRUE(pool.count <= MAX_CARDS);
    
    // Check that all cards in pool are valid
    for (int i = 0; i < pool.count; i++) {
        TEST_ASSERT_TRUE(is_valid_card(&pool.cards[i]));
    }
}

// Test getting card by ID
void test_get_card_by_id(void) {
    CardPool pool;
    init_card_pool(&pool);
    
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    Card* invalid = get_card_by_id(&pool, 999);
    
    TEST_ASSERT_NOT_NULL(strike);
    TEST_ASSERT_EQUAL(CARD_STRIKE, strike->id);
    TEST_ASSERT_EQUAL_STRING("Strike", strike->name);
    
    TEST_ASSERT_NOT_NULL(defend);
    TEST_ASSERT_EQUAL(CARD_DEFEND, defend->id);
    TEST_ASSERT_EQUAL_STRING("Defend", defend->name);
    
    TEST_ASSERT_NULL(invalid);
}

// Test card effects
void test_card_effects(void) {
    CardPool pool;
    init_card_pool(&pool);
    
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    Card* heal = get_card_by_id(&pool, CARD_HEAL);
    
    TEST_ASSERT_EQUAL(6, strike->damage);
    TEST_ASSERT_EQUAL(0, strike->block);
    TEST_ASSERT_EQUAL(0, strike->heal);
    
    TEST_ASSERT_EQUAL(0, defend->damage);
    TEST_ASSERT_EQUAL(5, defend->block);
    TEST_ASSERT_EQUAL(0, defend->heal);
    
    TEST_ASSERT_EQUAL(0, heal->damage);
    TEST_ASSERT_EQUAL(0, heal->block);
    TEST_ASSERT_EQUAL(4, heal->heal);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_card_creation);
    RUN_TEST(test_card_comparison);
    RUN_TEST(test_card_validation);
    RUN_TEST(test_card_pool_init);
    RUN_TEST(test_get_card_by_id);
    RUN_TEST(test_card_effects);
    
    return UNITY_END();
}

