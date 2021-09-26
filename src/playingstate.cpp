#include <iostream>
#include <SDL.h>
#include "PlayingState.hpp"
#include "Text.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "Constants.hpp"

extern RenderWindow window;

PlayingState::PlayingState()
:player_to_move_text_({768, 700}, {0, 0}, 50), player2_to_move_(false) {
	state_name_ = "PlayingState";
	player_to_move_text_.openFont("res/fixedsys.ttf", 50);

	blue_tex_ = window.loadTexture("res/blue.png");
	red_tex_ = window.loadTexture("res/red.png");
	board_.resize(NUM_ROWS);
	for (int i = 0; i < NUM_ROWS; i++) {
		board_[i].reserve(NUM_COLS);
	}

	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			board_[i].emplace_back(Entity({(CELL_SIZE * (j+1.0f)) + BOARD_X_OFFSET + CELL_SIZE / 8, (CELL_SIZE * (i+1.0f)) + BOARD_Y_OFFSET + CELL_SIZE / 8}, {0, 0}, {0, 0, 100, 100}, {0, 0, 100, 100}, nullptr, red_tex_));
			board_[i][j].setScale(0.8);
			board_[i][j].setInvisible();
			//test what the sprites look like
			//if ((i % 2) == 0) {
			//	board_[i][j].setFgTex(blue_tex_);
			//}
		}
	}

	mouse_down_ = false;
	updatePlayerMoveText();
}

void PlayingState::nextPlayerToMove() {
	player2_to_move_ = !player2_to_move_;
	updatePlayerMoveText();
}

void PlayingState::updatePlayerMoveText() {
	if (player2_to_move_) {
		player_to_move_text_.loadFontTexture(BLUE, "Player 2");
	} else {
		player_to_move_text_.loadFontTexture(RED, "Player 1");
	}
}

void PlayingState::placeToken(int col) {
	bool row_set = false;
	int row;
	for (int i = 0; i < NUM_ROWS; i++) {
		if (board_[i][col].getVisible()) {
			row = i - 1;
			if (row < 0) {
				return;
			}
			row_set = true;
			break;
		}
	}
	if (!row_set) {
		row	= NUM_ROWS - 1;
	}
	if (player2_to_move_) {
		board_[row][col].setFgTex(blue_tex_);
	} else {
		board_[row][col].setFgTex(red_tex_);
	}
	board_[row][col].setVisible();
	nextPlayerToMove();
}

bool PlayingState::checkValidMouseLocation() {
	if (mouse_x_ > BOARD_X_OFFSET + CELL_SIZE && mouse_x_ < (CELL_SIZE * (NUM_COLS + 2)) + LINE_THICKNESS) {
		printf("Valid\n");
		return true;
	}
		printf("Invalid\n");
	return false;
}

void PlayingState::handleInput(Game& game, const SDL_Event& event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_SPACE:
						nextPlayerToMove();
						break;
					case SDLK_1:
						placeToken(0);
						break;
					case SDLK_2:
						placeToken(1);
						break;
					case SDLK_3:
						placeToken(2);
						break;
					case SDLK_4:
						placeToken(3);
						break;
					case SDLK_5:
						placeToken(4);
						break;
					case SDLK_6:
						placeToken(5);
						break;
					case SDLK_7:
						placeToken(6);
						break;
				}
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					if (event.type == SDL_MOUSEBUTTONDOWN) {
						SDL_GetMouseState(&mouse_x_, &mouse_y_);
						mouse_down_ = true;
					} else if (event.type == SDL_MOUSEBUTTONUP && mouse_down_) {
						mouse_down_ = false;
						if (checkValidMouseLocation()) {

						}
					} else {
						mouse_down_ = false;
					}
					break;
		}
}

void PlayingState::update(Game& game) {
	window.clear(BUBBLE, 0xFF);

	//text to show which player's move it is
	window.render(player_to_move_text_);

	//color bg before rendering grid
	for (int i = 1; i < NUM_ROWS + 1; i++) {
		for (int j = 1; j < NUM_COLS + 1; j++) {
		window.renderRect({(CELL_SIZE * j) + LINE_THICKNESS + BOARD_X_OFFSET, (CELL_SIZE * i) + LINE_THICKNESS + BOARD_Y_OFFSET, CELL_SIZE, CELL_SIZE}, WHITE);
		}
	}
	//columns
	for (int i = 1; i < 9; i++) {
		window.renderRect({(CELL_SIZE * i) + BOARD_X_OFFSET, CELL_SIZE + BOARD_Y_OFFSET, LINE_THICKNESS, CELL_SIZE * (NUM_COLS - 1)}, BLACK);
	}
	//rows
	for (int i = 1; i < 8; i++) {
		int width = CELL_SIZE * (NUM_ROWS + 1);
		if (i == 7) {
			width += LINE_THICKNESS;
		}
		window.renderRect({CELL_SIZE + BOARD_X_OFFSET, (CELL_SIZE * i) + BOARD_Y_OFFSET, width, LINE_THICKNESS}, BLACK);
	}
	//tokens
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			window.render(board_[i][j].renderFgRectInfo(), board_[i][j].getFgTex());
		}
	}
	window.display();
	window.showWindow();
}