#pragma once
#include <SDL.h>

//#define DEBUG_CONTROLS 1

enum class game_mode {
	ONE_PLAYER,
	TWO_PLAYER,
	ONLINE
};

constexpr int SCREEN_WIDTH = 1024;
constexpr int SCREEN_HEIGHT = 768;

constexpr int EMPTY_PIECE = 0;
constexpr int PLAYER_PIECE = 1;
//actually, player two is the AI piece
//i'm not going to search and replace the term out though, i'm used to it
constexpr int AI_PIECE = 2;

constexpr int SEARCH_DEPTH = 5;

//constexpr bool DEBUG_PRINT = true;
constexpr bool DEBUG_PRINT = false;
constexpr bool RANDOM_PLAYER_TO_MOVE = false;
constexpr bool AI_TUNING = false;

constexpr int COLOR_CYCLE_SPEED = 200;

constexpr SDL_Color MODS_BLUE = {0x00, 0xCE, 0xFF};
constexpr SDL_Color GAINSBORO = {220, 220, 220};
constexpr SDL_Color DISNEY_GOOFY_COLOR = {237, 126, 32};
constexpr SDL_Color LIGHT_PURPLE = {203, 195, 227};
constexpr SDL_Color SKY_BLUE = {135, 206, 235};
constexpr SDL_Color BUBBLE = {0xD7, 0xEA, 0xF9};
constexpr SDL_Color CLASSIC_BACKGROUND = {12, 67, 114};
constexpr SDL_Color NEW_BACKGROUND = {64, 114, 148};
constexpr SDL_Color RED = {255, 0, 0};
constexpr SDL_Color GREEN = {0, 255, 0};
constexpr SDL_Color LIGHT_GREEN = {144, 238, 144};
constexpr SDL_Color FERN = {113, 188, 120};
constexpr SDL_Color JUNGLE_GREEN = {41, 171, 135};
constexpr SDL_Color HONEYDEW = {240, 255, 240};
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
constexpr int BOARD_Y_OFFSET = (SCREEN_HEIGHT / 2) - ((CELL_SIZE * (NUM_ROWS + 2)) / 2) + 30;