# Deckbuilder Roguelike Game

A deckbuilder roguelike game built in pure C with SDL2, following Test-Driven Development (TDD) methodology.

## Project Status

✅ **Completed Systems:**
- Random Number Generation (xorshift64 algorithm)
- Card System (20+ unique cards with effects)
- Deck Management (draw/discard/shuffle mechanics)
- Player System (HP, energy, block, status effects)
- Enemy System (AI patterns, intents, actions)
- Combat System (turn-based combat with card playing)

🔄 **Next Phase:**
- SDL2 Rendering (visual interface)
- Game Loop (main game state management)
- Roguelike Structure (floors, encounters)

## Architecture

**Pure C Implementation:**
- All game logic in C (no assembly)
- SDL2 for graphics and rendering
- Unity test framework for unit tests
- TDD approach: write tests first, then implementation

## Project Structure

```
assembly-test/
├── src/
│   ├── main.c              # Entry point, main game loop
│   ├── game.c/h            # Core game state management
│   ├── cards.c/h           # Card system (types, effects)
│   ├── combat.c/h          # Combat resolution
│   ├── deck.c/h            # Deck management
│   ├── enemy.c/h           # Enemy AI and encounters
│   ├── render.c/h          # SDL2 rendering
│   ├── ui.c/h              # UI and menus
│   ├── random.c/h          # RNG utilities
│   └── utils.c/h           # Helper functions
├── tests/
│   ├── test_cards.c        # Card system tests
│   ├── test_deck.c         # Deck management tests
│   ├── test_combat.c       # Combat system tests
│   ├── test_enemy.c        # Enemy AI tests
│   ├── test_random.c       # RNG tests
│   ├── unity.h             # Unity test framework
│   ├── unity.c             # Unity test framework
│   └── test_runner.c       # Basic test runner
├── assets/                 # Fonts, sprites
├── Makefile                # Build for game and tests
└── README.md              # This file
```

## Building and Running

### Prerequisites
- macOS with Homebrew
- SDL2, SDL2_ttf, SDL2_mixer
- pkg-config

### Installation
```bash
# Install dependencies
make install-deps

# Build and run tests
make test

# Build the game (when implemented)
make game

# Run the game (when implemented)
make run
```

## Test-Driven Development

This project follows TDD methodology:

1. **Red**: Write failing test for desired functionality
2. **Green**: Write minimal code to pass the test
3. **Refactor**: Clean up code while keeping tests green

### Running Tests
```bash
make test
```

All tests pass with comprehensive coverage of:
- Random number generation
- Card creation and validation
- Deck operations (add, draw, shuffle, discard)
- Player system (HP, energy, block, status effects)
- Enemy system (AI patterns, intents, actions)
- Combat system (turn-based combat, card playing)
- Card pool management

**Total: 53 tests passing** 🎉

## Game Design

### Card Types
- **Attack Cards**: Deal damage to enemies
- **Skill Cards**: Provide block, healing, or utility effects
- **Power Cards**: Provide persistent effects

### Core Mechanics
- Turn-based combat
- Energy system (3 energy per turn)
- Deck building between encounters
- Roguelike progression (permadeath, procedural generation)

### Sample Cards
- **Strike** (1 energy): Deal 6 damage
- **Defend** (1 energy): Gain 5 block
- **Bash** (2 energy): Deal 8 damage
- **Heal** (1 energy): Restore 4 HP
- **Fireball** (3 energy): Deal 15 damage

## Development Progress

### Phase 1: Foundation ✅
- [x] Project setup with SDL2 and Unity
- [x] Random number generation system
- [x] Card system with 20+ cards
- [x] Deck management with shuffle/draw/discard

### Phase 2: Game Logic ✅
- [x] Player system (HP, energy, status effects)
- [x] Enemy system (AI patterns, intents)
- [x] Combat system (turn-based, damage calculation)
- [x] Card effects and interactions

### Phase 3: Rendering
- [ ] SDL2 window and basic rendering
- [ ] Card UI with text and graphics
- [ ] Health bars and energy display
- [ ] Mouse input and interactions

### Phase 4: Game Loop
- [ ] Roguelike structure (floors, encounters)
- [ ] Deck building mechanics
- [ ] Game state management
- [ ] Polish and balance

## Technical Details

### Random Number Generation
Uses xorshift64 algorithm for fast, high-quality random numbers:
```c
uint64_t next_random(void);
uint64_t random_range(uint64_t max);
```

### Card System
Comprehensive card management with validation:
```c
typedef struct {
    CardID id;
    char name[32];
    int cost;
    CardType type;
    int damage;
    int block;
    int heal;
} Card;
```

### Deck Management
Full deck operations with Fisher-Yates shuffle:
```c
typedef struct {
    Card draw_pile[MAX_DECK_SIZE];
    Card discard_pile[MAX_DECK_SIZE];
    Card hand[MAX_HAND_SIZE];
    int draw_pile_count;
    int discard_pile_count;
    int hand_count;
} Deck;
```

## Contributing

This project follows TDD principles. When adding new features:

1. Write tests first
2. Implement minimal code to pass tests
3. Refactor while keeping tests green
4. Ensure all tests pass before committing

## License

This project is for educational purposes demonstrating TDD in C game development.
