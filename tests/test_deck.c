#include "unity.h"
#include "deck.h"
#include "cards.h"
#include <stdio.h>

void setUp(void) {
    // Set up test fixtures before each test
}

void tearDown(void) {
    // Clean up after each test
}

// Test deck creation and initialization
void test_deck_creation(void) {
    Deck deck;
    init_deck(&deck);
    
    TEST_ASSERT_EQUAL(0, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(0, deck.discard_pile_count);
    TEST_ASSERT_EQUAL(0, deck.hand_count);
}

// Test adding cards to deck
void test_add_card_to_deck(void) {
    Deck deck;
    init_deck(&deck);
    
    CardPool pool;
    init_card_pool(&pool);
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    
    TEST_ASSERT_TRUE(add_card_to_deck(&deck, strike));
    TEST_ASSERT_EQUAL(1, deck.draw_pile_count);
    
    TEST_ASSERT_TRUE(add_card_to_deck(&deck, defend));
    TEST_ASSERT_EQUAL(2, deck.draw_pile_count);
}

// Test drawing cards from deck
void test_draw_card(void) {
    Deck deck;
    init_deck(&deck);
    
    CardPool pool;
    init_card_pool(&pool);
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    
    add_card_to_deck(&deck, strike);
    add_card_to_deck(&deck, defend);
    
    Card* drawn1 = draw_card(&deck);
    TEST_ASSERT_NOT_NULL(drawn1);
    TEST_ASSERT_EQUAL(1, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(1, deck.hand_count);
    
    Card* drawn2 = draw_card(&deck);
    TEST_ASSERT_NOT_NULL(drawn2);
    TEST_ASSERT_EQUAL(0, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(2, deck.hand_count);
}

// Test drawing from empty deck
void test_draw_from_empty_deck(void) {
    Deck deck;
    init_deck(&deck);
    
    Card* drawn = draw_card(&deck);
    TEST_ASSERT_NULL(drawn);
    TEST_ASSERT_EQUAL(0, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(0, deck.hand_count);
}

// Test shuffling deck
void test_shuffle_deck(void) {
    Deck deck;
    init_deck(&deck);
    
    CardPool pool;
    init_card_pool(&pool);
    
    // Add 5 different cards to make shuffling more likely to change order
    Card* cards[] = {
        get_card_by_id(&pool, CARD_STRIKE),
        get_card_by_id(&pool, CARD_DEFEND),
        get_card_by_id(&pool, CARD_BASH),
        get_card_by_id(&pool, CARD_HEAL),
        get_card_by_id(&pool, CARD_SLASH)
    };
    
    for (int i = 0; i < 5; i++) {
        add_card_to_deck(&deck, cards[i]);
    }
    
    // Store original order
    Card original_order[5];
    for (int i = 0; i < 5; i++) {
        original_order[i] = deck.draw_pile[i];
    }
    
    // Shuffle multiple times to increase chance of order change
    shuffle_deck(&deck);
    shuffle_deck(&deck);
    shuffle_deck(&deck);
    
    // Check that order changed (very unlikely to be the same after 3 shuffles)
    bool order_changed = false;
    for (int i = 0; i < 5; i++) {
        if (!cards_equal(&original_order[i], &deck.draw_pile[i])) {
            order_changed = true;
            break;
        }
    }
    TEST_ASSERT_TRUE(order_changed);
    
    // Check that all cards are still there
    TEST_ASSERT_EQUAL(5, deck.draw_pile_count);
}

// Test reshuffling discard pile into draw pile
void test_reshuffle_discard(void) {
    Deck deck;
    init_deck(&deck);
    
    CardPool pool;
    init_card_pool(&pool);
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    
    // Add cards and draw them to hand
    add_card_to_deck(&deck, strike);
    add_card_to_deck(&deck, defend);
    
    draw_card(&deck);
    draw_card(&deck);
    
    TEST_ASSERT_EQUAL(0, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(2, deck.hand_count);
    TEST_ASSERT_EQUAL(0, deck.discard_pile_count);
    
    // Discard both cards to discard pile
    discard_card(&deck, 0); // Discard first card
    discard_card(&deck, 0); // Discard remaining card (now at index 0)
    
    TEST_ASSERT_EQUAL(0, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(0, deck.hand_count);
    TEST_ASSERT_EQUAL(2, deck.discard_pile_count);
    
    // Try to draw again - should reshuffle
    Card* drawn = draw_card(&deck);
    TEST_ASSERT_NOT_NULL(drawn);
    TEST_ASSERT_EQUAL(1, deck.draw_pile_count);
    TEST_ASSERT_EQUAL(1, deck.hand_count);
    TEST_ASSERT_EQUAL(0, deck.discard_pile_count);
}

// Test deck count functions
void test_deck_counts(void) {
    Deck deck;
    init_deck(&deck);
    
    CardPool pool;
    init_card_pool(&pool);
    Card* strike = get_card_by_id(&pool, CARD_STRIKE);
    Card* defend = get_card_by_id(&pool, CARD_DEFEND);
    
    add_card_to_deck(&deck, strike);
    add_card_to_deck(&deck, defend);
    
    TEST_ASSERT_EQUAL(2, get_deck_total_count(&deck));
    
    draw_card(&deck);
    TEST_ASSERT_EQUAL(2, get_deck_total_count(&deck));
    TEST_ASSERT_EQUAL(1, get_draw_pile_count(&deck));
    TEST_ASSERT_EQUAL(1, get_hand_count(&deck));
    TEST_ASSERT_EQUAL(0, get_discard_pile_count(&deck));
}

// Test starter deck creation
void test_starter_deck(void) {
    Deck deck;
    CardPool pool;
    init_card_pool(&pool);
    
    create_starter_deck(&deck, &pool);
    
    TEST_ASSERT_TRUE(get_deck_total_count(&deck) > 0);
    TEST_ASSERT_TRUE(get_deck_total_count(&deck) <= MAX_DECK_SIZE);
    
    // Check that we have some basic cards
    bool has_strike = false;
    bool has_defend = false;
    
    for (int i = 0; i < deck.draw_pile_count; i++) {
        if (deck.draw_pile[i].id == CARD_STRIKE) has_strike = true;
        if (deck.draw_pile[i].id == CARD_DEFEND) has_defend = true;
    }
    
    TEST_ASSERT_TRUE(has_strike);
    TEST_ASSERT_TRUE(has_defend);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_deck_creation);
    RUN_TEST(test_add_card_to_deck);
    RUN_TEST(test_draw_card);
    RUN_TEST(test_draw_from_empty_deck);
    RUN_TEST(test_shuffle_deck);
    RUN_TEST(test_reshuffle_discard);
    RUN_TEST(test_deck_counts);
    RUN_TEST(test_starter_deck);
    
    return UNITY_END();
}
