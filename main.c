#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAP_SIZE_X 5
#define MAP_SIZE_Y 4
#define TOTAL_CARDS 20
#define TOTAL_PAIRS 10

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
    int map[MAP_SIZE_Y][MAP_SIZE_X];
    int cards[MAP_SIZE_Y][MAP_SIZE_X];

    Position opened_cards[2];
    Position cursor;

    int opened_count;
    int score;
    bool game_running;
} GameState;

int random_int(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

bool samePosition(Position a, Position b)
{
    return a.x == b.x && a.y == b.y;
}

void shuffleArray(int array[])
{
    for (int i = 0; i < TOTAL_CARDS; i++) {
        int randomIndex = random_int(0, TOTAL_CARDS - 1);

        int temp = array[i];
        array[i] = array[randomIndex];
        array[randomIndex] = temp;
    }
}

void initCards(GameState *state)
{
    int values[TOTAL_CARDS] = {
        0, 0,
        1, 1,
        2, 2,
        3, 3,
        4, 4,
        5, 5,
        6, 6,
        7, 7,
        8, 8,
        9, 9
    };

    shuffleArray(values);

    int index = 0;

    for (int y = 0; y < MAP_SIZE_Y; y++) {
        for (int x = 0; x < MAP_SIZE_X; x++) {
            state->cards[y][x] = values[index];
            index++;
        }
    }
}

GameState initializeGame()
{
    GameState state;

    state.cursor.x = 0;
    state.cursor.y = 0;

    state.opened_cards[0].x = -1;
    state.opened_cards[0].y = -1;
    state.opened_cards[1].x = -1;
    state.opened_cards[1].y = -1;

    state.opened_count = 0;
    state.score = 0;
    state.game_running = true;

    memset(state.map, INCOGNITO_CARD, sizeof(state.map));
    memset(state.cards, -1, sizeof(state.cards));

    initCards(&state);

    return state;
}

bool canUpdateCursor(Position cursor)
{
    return cursor.x >= 0 && cursor.x < MAP_SIZE_X
        && cursor.y >= 0 && cursor.y < MAP_SIZE_Y;
}

void printMap(GameState *state)
{
    printf("JOGO DA MEMORIA\n\n");
    printf("Use W A S D para mover\n");
    printf("Use X para abrir uma carta\n");
    printf("Use Q para sair\n\n");
    printf("Pontuacao: %d\n\n", state->score);

    for (int y = 0; y < MAP_SIZE_Y; y++) {
        for (int x = 0; x < MAP_SIZE_X; x++) {
            Position currentPosition = { .x = x, .y = y };

            bool isCursor = samePosition(state->cursor, currentPosition);

            if (state->map[y][x] == DISCOVERED_CARD) {
                if (isCursor) {
                    printf("[%d]", state->cards[y][x]);
                } else {
                    printf(" %d ", state->cards[y][x]);
                }
            } else if (state->map[y][x] == SELECTED_CARD) {
                if (isCursor) {
                    printf("[%d]", state->cards[y][x]);
                } else {
                    printf(" %d ", state->cards[y][x]);
                }
            } else {
                if (isCursor) {
                    printf("[.]");
                } else {
                    printf(" . ");
                }
            }
        }

        printf("\n");
    }

    printf("\n");
}

void closeOpenedCards(GameState *state)
{
    Position first = state->opened_cards[0];
    Position second = state->opened_cards[1];

    state->map[first.y][first.x] = INCOGNITO_CARD;
    state->map[second.y][second.x] = INCOGNITO_CARD;

    state->opened_cards[0].x = -1;
    state->opened_cards[0].y = -1;
    state->opened_cards[1].x = -1;
    state->opened_cards[1].y = -1;

    state->opened_count = 0;
}

void markOpenedCardsAsDiscovered(GameState *state)
{
    Position first = state->opened_cards[0];
    Position second = state->opened_cards[1];

    state->map[first.y][first.x] = DISCOVERED_CARD;
    state->map[second.y][second.x] = DISCOVERED_CARD;

    state->opened_cards[0].x = -1;
    state->opened_cards[0].y = -1;
    state->opened_cards[1].x = -1;
    state->opened_cards[1].y = -1;

    state->opened_count = 0;
    state->score++;
}

void waitForEnter()
{
    getchar();
    getchar();
}

void checkMatch(GameState *state)
{
    if (state->opened_count < 2) {
        return;
    }

    Position first = state->opened_cards[0];
    Position second = state->opened_cards[1];

    int firstValue = state->cards[first.y][first.x];
    int secondValue = state->cards[second.y][second.x];

    if (firstValue == secondValue) {
        markOpenedCardsAsDiscovered(state);
    } else {
        clearScreen();
        printMap(state);

        printf("As cartas sao diferentes!\n");
        printf("Pressione ENTER para continuar...");

        waitForEnter();

        closeOpenedCards(state);
    }
}

void openCard(GameState *state)
{
    int x = state->cursor.x;
    int y = state->cursor.y;

    if (state->map[y][x] == DISCOVERED_CARD) {
        return;
    }

    if (state->map[y][x] == SELECTED_CARD) {
        return;
    }

    if (state->opened_count >= 2) {
        return;
    }

    state->map[y][x] = SELECTED_CARD;

    state->opened_cards[state->opened_count] = state->cursor;
    state->opened_count++;

    checkMatch(state);
}

bool hasWon(GameState *state)
{
    return state->score == TOTAL_PAIRS;
}

void handleInput(GameState *state)
{
    Position cursor = state->cursor;

    char input;
    scanf("%c", &input);

    switch (input) {
        case 'w':
        case 'W':
            cursor.y--;
            break;

        case 's':
        case 'S':
            cursor.y++;
            break;

        case 'a':
        case 'A':
            cursor.x--;
            break;

        case 'd':
        case 'D':
            cursor.x++;
            break;

        case 'x':
        case 'X':
            openCard(state);
            break;

        case 'q':
        case 'Q':
            state->game_running = false;
            break;

        default:
            break;
    }

    if (canUpdateCursor(cursor)) {
        state->cursor = cursor;
    }
}

int main(void)
{
    srand(time(NULL));

    GameState state = initializeGame();

    while (state.game_running) {
        clearScreen();
        printMap(&state);

        if (hasWon(&state)) {
            printf("Parabens! Voce venceu!\n");
            break;
        }

        handleInput(&state);
    }

    return 0;
}
