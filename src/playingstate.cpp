#include <iostream>
#include <SDL.h>
#include "PlayingState.hpp"
#include "Text.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "Constants.hpp"
#include "BasicButton.hpp"
#include <random>

extern RenderWindow window;

PlayingState::PlayingState()
:GameState("PlayingState"),
player_to_move_text_({768, 715}, {0, 0}),
won_(false), drawn_(false),
win_text_({SCREEN_WIDTH / 2, 250}, {0, 0}),
draw_text_({SCREEN_WIDTH / 2, 250}, {0, 0}),
restart_button_(BasicButton({SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, WHITE, GREEN, 5, "Play Again")),
back_to_intro_button_(BasicButton({SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, GAINSBORO, RED, 5, "Quit")) 
{
	player_to_move_text_.openFont("res/fixedsys.ttf", 50);
	win_text_.openFont("res/fixedsys.ttf", 50);
	draw_text_.openFont("res/fixedsys.ttf", 50);
	blue_tex_ = window.loadTexture("res/blue.png");
	red_tex_ = window.loadTexture("res/red.png");
	blue_arrow_ = window.loadTexture("res/bluearrow.png");
	red_arrow_ = window.loadTexture("res/redarrow.png");

	for (int i = 0; i < 7; i++) {
		numbers_text_.push_back({{BOARD_X_OFFSET + CELL_SIZE / 2 - LINE_THICKNESS + static_cast<float>(CELL_SIZE * (i + 1)) - 7, 0}, {0, 0}});
		display_arrows_.push_back({{BOARD_X_OFFSET + CELL_SIZE + static_cast<float>(CELL_SIZE * i), 25}, {0, 0}, {0, 0, 100, 100}, {0, 0, 100, 100}, nullptr, red_arrow_});
		char number[2];
		sprintf(number, "%d", i + 1);
		numbers_text_[i].openFont("res/fixedsys.ttf", 35);
		numbers_text_[i].loadFontTexture(BLACK, number);
	}

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

;

#ifdef DEBUG_CONTROLS
	player_color_lock_ = false;
#endif

	current_mouse_location_ = 0;
	bound_to_board_ = false;
	bound_to_restart_screen_ = false;

	colors_ = {BUBBLE, SKY_BLUE, GAINSBORO, CLASSIC_BACKGROUND, NEW_BACKGROUND, RED, GREEN, BLUE, CYAN, PEACH, PURPLE, ORANGE_RED, WHITE, BLACK, PASTEL_BLUE, LIME_GREEN};
	color_index_location_ = 0;

	randomPlayerToMove();
	updatePlayerMoveText();
}

void PlayingState::nextBgColor() {
	color_index_location_++;
	if (color_index_location_ > colors_.size() - 1) {
		color_index_location_ = 0;
	}
}

void PlayingState::randomPlayerToMove() {
	std::random_device seeder;
	std::mt19937_64 rd;
	rd.seed(seeder());
	if ((rd() % 2)) {
		player2_to_move_ = true;
	} else {
		player2_to_move_ = false;
	}
}

void PlayingState::nextPlayerToMove(bool force) {
#ifdef DEBUG_CONTROLS
	if (!player_color_lock_ || force) {
#endif
	player2_to_move_ = !player2_to_move_;
	updatePlayerMoveText();
#ifdef DEBUG_CONTROLS
	}
#endif
}

void PlayingState::updatePlayerMoveText() {
	if (player2_to_move_) {
		player_to_move_text_.loadFontTexture(BLUE, "Player 2");
	} else {
		player_to_move_text_.loadFontTexture(RED, "Player 1");
	}
}

SDL_Texture *PlayingState::getArrowTex() {
	if (player2_to_move_) {
		return blue_arrow_;
	} else {
		return red_arrow_;
	}
}

void PlayingState::placeToken(int col) {
	if (won_ || drawn_) {
		return;
	}
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

void PlayingState::checkWinOrDraw() {
	bool empty_space_found = false;
	bool red_won = false;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (!board_[i][j].getVisible()) {
				empty_space_found = true;
			}
			//horizontal
			if (!won_ && j + 3 < NUM_COLS) {
				if (board_[i][j].getVisible() && board_[i][j+1].getVisible() && board_[i][j+2].getVisible() && board_[i][j+3].getVisible()) {
					if (board_[i][j].getFgTex() == board_[i][j+1].getFgTex() && board_[i][j+1].getFgTex() == board_[i][j+2].getFgTex() && board_[i][j+2].getFgTex() == board_[i][j+3].getFgTex()) {
						won_ = true;
						if (board_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}
			//vertical
			if (!won_ && i + 3 < NUM_ROWS) {
				if (board_[i][j].getVisible() && board_[i+1][j].getVisible() && board_[i+2][j].getVisible() && board_[i+3][j].getVisible()) {
					if (board_[i][j].getFgTex() == board_[i+1][j].getFgTex() && board_[i+1][j].getFgTex() == board_[i+2][j].getFgTex() && board_[i+2][j].getFgTex() == board_[i+3][j].getFgTex()) {
						won_ = true;
						if (board_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}
			//backslash slash diagonal
			if (!won_ && i + 3 < NUM_ROWS && j + 3 < NUM_COLS) {
				if (board_[i][j].getVisible() && board_[i+1][j+1].getVisible() && board_[i+2][j+2].getVisible() && board_[i+3][j+3].getVisible()) {
					if (board_[i][j].getFgTex() == board_[i+1][j+1].getFgTex() && board_[i+1][j+1].getFgTex() == board_[i+2][j+2].getFgTex() && board_[i+2][j+2].getFgTex() == board_[i+3][j+3].getFgTex()) {
						won_ = true;
						if (board_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}

			//forward slash diagonal
			if (!won_ && i - 3 >= 0 && j + 3 < NUM_COLS) {
				if (board_[i][j].getVisible() && board_[i-1][j+1].getVisible() && board_[i-2][j+2].getVisible() && board_[i-3][j+3].getVisible()) {
					if (board_[i][j].getFgTex() == board_[i-1][j+1].getFgTex() && board_[i-1][j+1].getFgTex() == board_[i-2][j+2].getFgTex() && board_[i-2][j+2].getFgTex() == board_[i-3][j+3].getFgTex()) {
						won_ = true;
						if (board_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}
		}
	}
	if (won_) {
		win(red_won);
	}
	if (!empty_space_found) {
		draw();
	}
}

void PlayingState::win(bool red_won) {
	if (red_won) {
		win_text_.loadFontTexture(RED, "Player 1 wins!");
		printf("RED WIN\n");
	} else {
		win_text_.loadFontTexture(BLUE, "Player 2 wins!");
		printf("BLUE WIN\n");
	}
}

void PlayingState::draw() {
	printf("DRAW\n");
}

void PlayingState::resetGame() {
	for (int i = 0; i < NUM_ROWS; i++){
		for (int j = 0; j < NUM_COLS; j++){
			board_[i][j].setInvisible();
		}
	}
	won_ = false;
	drawn_ = false;
}

bool PlayingState::checkValidMouseLocation() {
	if (mouse_x_ > BOARD_X_OFFSET + CELL_SIZE && mouse_x_ < BOARD_X_OFFSET + (CELL_SIZE * (NUM_COLS + 1)) + LINE_THICKNESS) {
		//printf("Valid\n");
		return true;
	}
		//printf("Invalid\n");
	return false;
}

int PlayingState::parseMouseLocation() {
	int mouse_relative = mouse_x_ - BOARD_X_OFFSET;
	int position = (mouse_relative / CELL_SIZE) - 1;
	//clicking on the final line will crash everything unless we do this, or some other amazing solution that we won't bother with
	if (position == 7) {
		position = 6;
	}
	//printf("%d\n", position);
	return position;
}

#ifdef DEBUG_CONTROLS
void PlayingState::nearlyFillBoard() {
	srand(time(0));
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS - 1; j++) {
			int num = (rand() % 4) + 2;
			switch (num) {
				case 0:
					board_[i][j].setFgTex(red_tex_);
					break;
				case 1:
					board_[i][j].setFgTex(blue_tex_);
					break;
				case 2:
					board_[i][j].setFgTex(red_arrow_);
					break;
				case 3:
					board_[i][j].setFgTex(blue_arrow_);
					break;
			}
			board_[i][j].setVisible();
		}
	}
}
#endif

void PlayingState::handleInput(Game& game, const SDL_Event& event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
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
					case SDLK_c:
						nextBgColor();
						break;
#ifdef DEBUG_CONTROLS
					case SDLK_SPACE:
						nextPlayerToMove(true);
						break;
					case SDLK_MINUS:
						resetGame();
						break;
					case SDLK_9:
						game.introState();
						break;
					case SDLK_a:
						player_color_lock_ = !player_color_lock_;
						break;
					case SDLK_l:
						nearlyFillBoard();
						break;
#endif
				}
				case SDL_MOUSEMOTION:
					SDL_GetMouseState(&mouse_x_, &mouse_y_);
					if (checkValidMouseLocation()) {
						current_mouse_location_ = parseMouseLocation();
					} else {
						current_mouse_location_ = -1;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					if (!won_ && !drawn_) {
						bound_to_board_ = true;
					} else {
						bound_to_restart_screen_ = true;
					}
					break;
				case SDL_MOUSEBUTTONUP:
					if (bound_to_restart_screen_) {
						if (restart_button_.clicked(mouse_x_, mouse_y_)) {
							resetGame();
						} else if (back_to_intro_button_.clicked(mouse_x_, mouse_y_)) {
							resetGame();
							game.introState();
						}
					}
					if (bound_to_board_) {
						if (checkValidMouseLocation() && !won_ && !drawn_) {
							//parseMouseLocation();
							placeToken(parseMouseLocation());
							checkWinOrDraw();
						}
					}
					bound_to_board_ = false;
					bound_to_restart_screen_ = false;
					break;
		}
}

void PlayingState::update(Game& game) {
	window.clear(colors_[color_index_location_], 0xFF);

	//text to show which player's move it is
	window.render(player_to_move_text_);
	//label each column with a number that can be pressed as an alternative to mouse controls
	for (int i = 0; i < 7; i++) {
		window.render(numbers_text_[i]);
		if (current_mouse_location_ != -1 && !won_ && !drawn_) {
			window.render(display_arrows_[current_mouse_location_].renderFgRectInfo(), getArrowTex());
		}
	}

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
	if (won_ || drawn_) {
		window.renderBlendedRect({0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, {0, 0, 0, 220});
		window.render(restart_button_);
		window.render(back_to_intro_button_);
	}
	if (won_) {
		window.render(win_text_, true);
	}
	if (drawn_) {
		window.render(draw_text_, true);
	}
	window.display();
	window.showWindow();
}