#include <SDL.h>

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

constexpr SDL_Color GAINSBORO = {220, 220, 220};
constexpr SDL_Color SKY_BLUE = {135, 206, 235};
constexpr SDL_Color BUBBLE = {0xD7, 0xEA, 0xF9};
constexpr SDL_Color CLASSIC_BACKGROUND = {12, 67, 114};
constexpr SDL_Color NEW_BACKGROUND = {64, 114, 148};
constexpr SDL_Color RED = {255, 0, 0};
constexpr SDL_Color GREEN = {0, 255, 0};
constexpr SDL_Color BLUE = {0, 0, 255};
constexpr SDL_Color CYAN = {0, 255, 255};
constexpr SDL_Color PEACH = {255, 229, 180};
constexpr SDL_Color PURPLE = {128, 0, 128};
constexpr SDL_Color ORANGE_RED = {255, 69, 0};
constexpr SDL_Color WHITE = {255, 255, 255};
constexpr SDL_Color BLACK = {0, 0, 0};
constexpr SDL_Color PASTEL_BLUE = {126, 214, 223};
constexpr SDL_Color LIME_GREEN = {186, 220, 88};

constexpr int CELL_SIZE = 100;
constexpr int LINE_THICKNESS = 5;
constexpr int NUM_ROWS = 6;
constexpr int NUM_COLS = 7;
constexpr int BOARD_X_OFFSET = (SCREEN_WIDTH / 2) - ((CELL_SIZE * (NUM_COLS + 2)) / 2);
constexpr int BOARD_Y_OFFSET = (SCREEN_HEIGHT / 2) - ((CELL_SIZE * (NUM_ROWS + 2)) / 2);