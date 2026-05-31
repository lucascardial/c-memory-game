#include <stdio.h>
#include <stdlib.h>
#define MAP_SIZE_X 5
#define MAP_SIZE_Y 4

enum CARDS {
    CARD_0 = 0,
    CARD_1 = 1,
    CARD_2 = 2,
    CARD_3 = 3,
    CARD_4 = 4,
    CARD_5 = 5,
    CARD_6 = 6,
    CARD_7 = 7,
    CARD_8 = 8,
    CARD_9 = 9,
};

enum ELEMENTS {
    INCOGNITO_CARD = 10,
    SELECTED_CARD = 11,
    DISCOVERED_CARD = 12
};

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    int map[MAP_SIZE_X][MAP_SIZE_Y];
    int cards[MAP_SIZE_X][MAP_SIZE_Y];

    Position opened_cards[2];
    Position cursor;
    int score;
} GameState;

int random_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

void initMap(GameState *state) {
    for (int x = 0; x < MAP_SIZE_X; x++) {
        for (int y = 0; y < MAP_SIZE_Y; y++) {
            state->map[x][y] = INCOGNITO_CARD;
        }
    }
}


void printMap(GameState *state) {
    const char *elements[12] = {
        [INCOGNITO_CARD] = "  .  ",
        [SELECTED_CARD] = "| . |",
        [0] = "  0  ",
        [1] = "  1  ",
        [2] = "  2  ",
        [3] = "  3  ",
        [4] = "  4  ",
        [5] = "  5  ",
        [6] = "  6  ",
        [7] = "  7  ",
        [8] = "  8  ",
        [9] = "  9  ",
    };

    for (int x = 0; x < MAP_SIZE_X; x++) {
        for (int y = 0; y < MAP_SIZE_Y; y++) {
            printf("%s", elements[state->map[x][y]]);
            if (y == MAP_SIZE_Y - 1) {
                printf("\n");
            }
        }
    }
}

void incrementScore(GameState *state) {
    state->score++;
}

GameState initialize_game() {
    GameState state;

    Position cursor = {.x = 0,.y = 0};
    int map [MAP_SIZE_X][MAP_SIZE_Y] = {0};
    state.cursor = cursor;
    state.score = 0;

    return state;
}

int main(void) {
    GameState state = initialize_game();
    initMap(&state);
    printMap(&state);
    return 0;
}
