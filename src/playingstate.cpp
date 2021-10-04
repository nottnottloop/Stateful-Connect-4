#include <iostream>
#include <SDL.h>
#include <random>
#include <algorithm>
#include "PlayingState.hpp"
#include "Text.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "Constants.hpp"
#include "BasicButton.hpp"

extern RenderWindow window;

PlayingState::PlayingState()
:GameState("PlayingState"),
player_to_move_text_({768, 715}, {0, 0}),
won_(false), drawn_(false),
win_text_({SCREEN_WIDTH / 2, 250}, {0, 0}),
draw_text_({SCREEN_WIDTH / 2, 250}, {0, 0}),
restart_button_(BasicButton({SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, BLACK, WHITE, GREEN, 5, "Play Again")),
back_to_intro_button_(BasicButton({SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, BLACK, GAINSBORO, RED, 5, "Quit")),
player_to_move_changed_(true)
{
	std::random_device seeder;
	rd_.seed(seeder());
	player_to_move_text_.openFont("res/fixedsys.ttf", 50);
	win_text_.openFont("res/fixedsys.ttf", 50);
	draw_text_.openFont("res/fixedsys.ttf", 50);
	draw_text_.loadFontTexture(PURPLE, "DRAW");
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

	board_entities_.resize(NUM_ROWS);
	for (int i = 0; i < NUM_ROWS; i++) {
		board_entities_[i].reserve(NUM_COLS);
	}

	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			board_entities_[i].emplace_back(Entity({(CELL_SIZE * (j+1.0f)) + BOARD_X_OFFSET + CELL_SIZE / 8, (CELL_SIZE * (i+1.0f)) + BOARD_Y_OFFSET + CELL_SIZE / 8}, {0, 0}, {0, 0, 100, 100}, {0, 0, 100, 100}, nullptr, red_tex_));
			board_entities_[i][j].setScale(0.8);
			board_entities_[i][j].setInvisible();
			//test what the sprites look like
			//if ((i % 2) == 0) {
			//	board_entities_[i][j].setFgTex(blue_tex_);
			//}
		}
	}

;

#ifdef DEBUG_CONTROLS
	player_color_lock_ = false;
#endif

	current_mouse_location_ = 0;
	bound_to_board_entities_ = false;
	bound_to_restart_screen_ = false;

	colors_ = {BUBBLE, SKY_BLUE, MODS_BLUE, GAINSBORO, CLASSIC_BACKGROUND, NEW_BACKGROUND, RED, GREEN, BLUE, CYAN, PEACH, PURPLE, ORANGE_RED, WHITE, BLACK, PASTEL_BLUE, LIME_GREEN};
	color_index_location_ = 0;

	randomPlayerToMove();
	resetGame();
}

void PlayingState::cycleColor(bool backward) {
	if (SDL_GetTicks() - color_cycle_ticks_ > COLOR_CYCLE_SPEED) {
		if (backward) {
			color_index_location_--;
			if (color_index_location_ < 0) {
				color_index_location_ = colors_.size() - 1;
			}
		} else {
			color_index_location_++;
			if (color_index_location_ > colors_.size() - 1) {
				color_index_location_ = 0;
			}
		}
		color_cycle_ticks_ = SDL_GetTicks();
	}
}

void PlayingState::randomPlayerToMove() {
	if ((rd_() % 2)) {
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
	if (game_mode_ != game_mode::ONE_PLAYER) {
		if (player2_to_move_) {
			player_to_move_text_.loadFontTexture(BLUE, "Player 2");
		} else {
			player_to_move_text_.loadFontTexture(RED, "Player 1");
		}
	} else {
		if (player2_to_move_) {
			player_to_move_text_.loadFontTexture(BLUE, "AI");
		} else {
			player_to_move_text_.loadFontTexture(RED, "Player");
		}
	}
}

SDL_Texture *PlayingState::getArrowTex() {
	if (player2_to_move_) {
		return blue_arrow_;
	} else {
		return red_arrow_;
	}
}

//this function changes the behaviour of player 1's attempt to place a token
//the 2 player, same computer code was written first so this allows that code to remain untouched
void PlayingState::tryToPlaceToken(int col) {
	if (game_mode_ == game_mode::TWO_PLAYER) {
		placeToken(col, board_, true);
	} else {
		if (!player2_to_move_) {
			placeToken(col, board_, true);
		}
	}
}

void PlayingState::aiMove() {
	if (!player2_to_move_ || won_ || drawn_) {
		return;
	}
	if (goofy_ai_) {
		int col = rd_() % NUM_COLS;
		printf("Goofy AI: %d\n", col);
		placeToken(col, board_, true);
	} else {
		int col;
		col = pickBestMove(AI_PIECE);
		placeToken(col, board_, true);
		printf("AI moves: %d\n\n", col);
	}
}

bool PlayingState::isValidColumn(int col) {
	bool valid = false;
	for (int i = 0; i < NUM_ROWS; i++) {
		if (!board_entities_[i][col].getVisible()) {
			valid = true;
		}
	}
	return valid;
}

//fake boards are tested when real is set to false for the purpose of ai
void PlayingState::placeToken(int col, Board &board, bool real) {
	if (won_ || drawn_) {
		return;
	}
	bool row_set = false;
	int row;
	for (int i = 0; i < NUM_ROWS; i++) {
		if (board[i][col] != 0) {
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
		board[row][col] = 2;
		if (real) {
			board_entities_[row][col].setFgTex(blue_tex_);
		}
	} else {
		board[row][col] = 1;
		if (real) {
			board_entities_[row][col].setFgTex(red_tex_);
		}
	}
	if (real) {
		board_entities_[row][col].setVisible();
		player_to_move_changed_ = true;
		nextPlayerToMove();
		checkWinAndDraw();
	}
}

//this function will check for whether the game is won or drawn
//it will also update the next column it thinks the AI should move to next
bool PlayingState::checkWinAndDraw(bool real) {
	bool empty_space_found = false;
	bool red_won = false;
	for (int i = 0; i < NUM_ROWS; i++) {
		for (int j = 0; j < NUM_COLS; j++) {
			if (!board_entities_[i][j].getVisible()) {
				empty_space_found = true;
			}
			//horizontal
			if (!won_ && j + 3 < NUM_COLS) {
				if (board_entities_[i][j].getVisible() && board_entities_[i][j+1].getVisible() && board_entities_[i][j+2].getVisible() && board_entities_[i][j+3].getVisible()) {
					if (board_entities_[i][j].getFgTex() == board_entities_[i][j+1].getFgTex() && board_entities_[i][j+1].getFgTex() == board_entities_[i][j+2].getFgTex() && board_entities_[i][j+2].getFgTex() == board_entities_[i][j+3].getFgTex()) {
						won_ = true;
						if (board_entities_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}
			//vertical
			if (!won_ && i + 3 < NUM_ROWS) {
				if (board_entities_[i][j].getVisible() && board_entities_[i+1][j].getVisible() && board_entities_[i+2][j].getVisible() && board_entities_[i+3][j].getVisible()) {
					if (board_entities_[i][j].getFgTex() == board_entities_[i+1][j].getFgTex() && board_entities_[i+1][j].getFgTex() == board_entities_[i+2][j].getFgTex() && board_entities_[i+2][j].getFgTex() == board_entities_[i+3][j].getFgTex()) {
						won_ = true;
						if (board_entities_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}
			//backslash slash diagonal
			if (!won_ && i + 3 < NUM_ROWS && j + 3 < NUM_COLS) {
				if (board_entities_[i][j].getVisible() && board_entities_[i+1][j+1].getVisible() && board_entities_[i+2][j+2].getVisible() && board_entities_[i+3][j+3].getVisible()) {
					if (board_entities_[i][j].getFgTex() == board_entities_[i+1][j+1].getFgTex() && board_entities_[i+1][j+1].getFgTex() == board_entities_[i+2][j+2].getFgTex() && board_entities_[i+2][j+2].getFgTex() == board_entities_[i+3][j+3].getFgTex()) {
						won_ = true;
						if (board_entities_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}

			//forward slash diagonal
			if (!won_ && i - 3 >= 0 && j + 3 < NUM_COLS) {
				if (board_entities_[i][j].getVisible() && board_entities_[i-1][j+1].getVisible() && board_entities_[i-2][j+2].getVisible() && board_entities_[i-3][j+3].getVisible()) {
					if (board_entities_[i][j].getFgTex() == board_entities_[i-1][j+1].getFgTex() && board_entities_[i-1][j+1].getFgTex() == board_entities_[i-2][j+2].getFgTex() && board_entities_[i-2][j+2].getFgTex() == board_entities_[i-3][j+3].getFgTex()) {
						won_ = true;
						if (board_entities_[i][j].getFgTex() == red_tex_) {
							red_won = true;
						}
					}
				}
			}
		}
	}
	if (won_ && real) {
		win(red_won);
		return;
	}
	if (!empty_space_found && real) {
		draw();
		return;
	}
	return won_ || !empty_space_found;
}

int PlayingState::evaluateWindow(Window window, int piece) {
	int opp_piece = PLAYER_PIECE;
	if (piece == PLAYER_PIECE) {
		opp_piece = AI_PIECE;
	}

	int score = 0;
	int piece_count = 0;
	int empty_count = 0;
	int opp_piece_count = 0;
	for (int i = 0; i < 4; i++) {
		if (window[i] == piece) {
			piece_count++;
		} else if (window[i] == opp_piece) {
			opp_piece_count++;
		} else if (window[i] == EMPTY_PIECE) {
			empty_count++;
		}
	}
	if (piece_count == 4) {
		score += 100;
	} else if (piece_count == 3 && empty_count == 1) {
		score += 10;
	} else if (piece_count == 2 && empty_count == 2) {
		score += 5;
	}

	if (opp_piece_count == 3 && empty_count == 1) {
		score -= 80;
	}
	return score;
}

int PlayingState::scorePosition(Board &board, int piece) {
	int score = 0;

	//center
	{
		std::array<int, 6> col_array;
		for (int r = 0; r < NUM_ROWS; r++) {
			col_array[r] = board[r][NUM_COLS / 2];
		}
		int center_count = 0;
		for (int i = 0; i < NUM_ROWS; i++) {
			if (col_array[i] == piece) {
				center_count++;
			}
		}
		score += center_count * 6;
	}
	 
	//horizontal
	for (int r = 0; r < NUM_ROWS; r++) {
		auto row_array = board[r];
		for (int c = 0; c < NUM_COLS - 3; c++) {
			std::array<int, 4> window_array;
			std::copy(row_array.begin() + c, row_array.begin() + c + 4, window_array.begin());
			score += evaluateWindow(window_array, piece);
		}
	}
	//vertical
	for (int c = 0; c < NUM_COLS; c++) {
		std::array<int, 6> col_array;
		for (int r = 0; r < NUM_ROWS; r++) {
			col_array[r] = board[r][c];
		}
		for (int r = 0; r < NUM_ROWS - 3; r++) {
			std::array<int, 4> window_array;
			std::copy(col_array.begin() + r, col_array.begin() + r + 4, window_array.begin());
			score += evaluateWindow(window_array, piece);
		}
	}
	//forwardslash diagonal
	for (int r = 0; r < NUM_ROWS - 3; r++) {
		for (int c = 0; c < NUM_COLS - 3; c++) {
			for (int i = 0; i < 4; i++) {
				std::array<int, 4> window_array;
				window_array[i] = board[r + i][c + i];
				score += evaluateWindow(window_array, piece);
			}
		}
	}
	//backslash diagonal
	for (int r = 0; r < NUM_ROWS - 3; r++) {
		for (int c = 0; c < NUM_COLS - 3; c++) {
			for (int i = 0; i < 4; i++) {
				std::array<int, 4> window_array;
				window_array[i] = board[r + 3 - i][c + i];
				score += evaluateWindow(window_array, piece);
			}
		}
	}
	return score;
}

bool PlayingState::isTerminalNode(Board board) {

}

int PlayingState::minimax(Board board, int depth, int maximising_player) {
	if (depth == 0 || terminal_node)

}

int PlayingState::pickBestMove(int piece) {
	int best_score = -10000;
	int best_column = rd_() % 7;
	for (int c = 0; c < NUM_COLS; c++) {
		if (!isValidColumn(c)) {
			continue;
		}
		auto temp_board = board_;
		placeToken(c, temp_board, false);
		int score = scorePosition(temp_board, piece);
		if (score > best_score) {
			best_score = score;
			best_column = c;
		}
	}
	return best_column;
}

void PlayingState::win(bool red_won) {
	if (game_mode_ != game_mode::ONE_PLAYER) {
		if (red_won) {
			win_text_.loadFontTexture(RED, "Player 1 wins!");
			printf("RED WIN\n");
		} else {
			win_text_.loadFontTexture(BLUE, "Player 2 wins!");
			printf("BLUE WIN\n");
		}
	} else {
		if (red_won) {
			win_text_.loadFontTexture(RED, "Player wins!");
			printf("RED WIN\n");
		} else {
			win_text_.loadFontTexture(BLUE, "AI wins!");
			printf("BLUE WIN\n");
		}
	}
}

void PlayingState::draw() {
	drawn_ = true;
	printf("DRAW\n");
}

void PlayingState::resetGame() {
	for (int i = 0; i < NUM_ROWS; i++){
		for (int j = 0; j < NUM_COLS; j++){
			board_entities_[i][j].setInvisible();
			board_[i][j] = 0;
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
					board_entities_[i][j].setFgTex(red_tex_);
					break;
				case 1:
					board_entities_[i][j].setFgTex(blue_tex_);
					break;
				case 2:
					board_entities_[i][j].setFgTex(red_arrow_);
					break;
				case 3:
					board_entities_[i][j].setFgTex(blue_arrow_);
					break;
			}
			board_entities_[i][j].setVisible();
		}
	}
}
#endif

void PlayingState::setGameMode(game_mode mode) {
	game_mode_ = mode;
}

void PlayingState::setGoofyAi(bool goofy) {
	if (goofy) {
		printf("goofy");
	}
	goofy_ai_ = goofy;
}

void PlayingState::setTurboAi(bool turbo) {
	if (turbo) {
		printf("turbo");
	}
	turbo_ai_ = turbo;
}

void PlayingState::handleInput(Game& game, const SDL_Event& event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_1:
						tryToPlaceToken(0);
						break;
					case SDLK_2:
						tryToPlaceToken(1);
						break;
					case SDLK_3:
						tryToPlaceToken(2);
						break;
					case SDLK_4:
						tryToPlaceToken(3);
						break;
					case SDLK_5:
						tryToPlaceToken(4);
						break;
					case SDLK_6:
						tryToPlaceToken(5);
						break;
					case SDLK_7:
						tryToPlaceToken(6);
						break;
					case SDLK_c:
						cycleColor(false);
						break;
					case SDLK_x:
						cycleColor(true);
						break;
#ifdef DEBUG_CONTROLS
					case SDLK_h:
						for (int col = 0; col < NUM_COLS; col++){
							printf("%d", isValidColumn(col));
						}
						printf("\n");
						break;
					case SDLK_j:
						printf("---\n");
						for (int row = 0; row < NUM_ROWS; row++) {
							for (int col = 0; col < NUM_COLS; col++) {
								printf("%d ", board_[row][col]);
							}
							printf("\n");
						}
						printf("---\n");
						break;
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
					case SDLK_d:
						draw();
						break;
#endif
				}
				break;
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
						bound_to_board_entities_ = true;
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
					if (bound_to_board_entities_) {
						if (checkValidMouseLocation() && !won_ && !drawn_) {
							//parseMouseLocation();
							tryToPlaceToken(parseMouseLocation());
						}
					}
					bound_to_board_entities_ = false;
					bound_to_restart_screen_ = false;
					break;
		}
}

void PlayingState::update(Game& game) {
	if (game_mode_ == game_mode::ONE_PLAYER) {
		aiMove();
	}
	window.clear(colors_[color_index_location_], 0xFF);

	//text to show which player's move it is
	if (player_to_move_changed_) {
		updatePlayerMoveText();
		player_to_move_changed_ = false;
	}
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
			window.render(board_entities_[i][j].renderFgRectInfo(), board_entities_[i][j].getFgTex());
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