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

void PlayingState::handleInput(Game& game, SDL_Event& event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_SPACE:
						nextPlayerToMove();
						break;
				}
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
	window.display();
	window.showWindow();
}