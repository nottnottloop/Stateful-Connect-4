#pragma once
#include <SDL.h>
#include <vector>
#include <array>
#include <random>
#include "GameState.hpp"
#include "Text.hpp"
#include "Entity.hpp"
#include "Constants.hpp"
#include "BasicButton.hpp"

class PlayingState : public GameState {
public:
	struct ScoreMove {
		int score, move;
	};
	using Board = std::array<std::array<int, NUM_COLS>, NUM_ROWS>;
	using Window = std::array<int, 4>;
	PlayingState();
	void handleInput(Game &game, const SDL_Event &event);
	void update(Game &game);
	void setGameMode(game_mode mode);
	void setGoofyAi(bool goofy);
	void setTurboAi(bool turbo);
	void setMinimaxAi(bool minimax);
	void changeColor(SDL_Color color);
	void cycleColor(bool backward);
	void randomPlayerToMove();
	void nextPlayerToMove(bool force = false);
	void updatePlayerMoveText();
	SDL_Texture *getArrowTex();
	void tryToPlaceToken(int col);
	void aiMove();
	bool isValidColumn(int col);
	std::vector<int> getValidLocations();
	int randomValidColumn();
	int getNextOpenRow(Board board, int col);
	void placeToken(int col, Board &board, bool real, int piece);
	bool checkWinAndDraw(Board board, int piece, bool real=true);
	int evaluateWindow(Window window, int piece);
	int scorePosition(Board &board, int piece);
	bool isTerminalNode(Board board);
	std::array<int, 2> minimax(Board board, int depth, int alpha, int beta, bool maximising_player);
	int pickBestMove(int piece);
	void win(bool red_won);
	void draw();
	void resetGame();
	bool checkValidMouseLocation();
	int parseMouseLocation();
	void debugPrintAi(bool maximising, int depth, int column, int new_score);
#ifdef DEBUG_CONTROLS
	void nearlyFillBoard();
#endif
private:
	std::mt19937_64 rd_;
	game_mode game_mode_;
	bool goofy_ai_;
	bool turbo_ai_;
	bool minimax_ai_;
	int next_ai_move_;
	int color_index_location_;
	std::vector<SDL_Color> colors_;
	SDL_Color current_color_;
	Uint32 color_cycle_ticks_;
	bool won_;
	bool drawn_;
	bool player2_to_move_;
	bool player_to_move_changed_;
	Text player_to_move_text_;
	std::vector<Text> numbers_text_;
	Text win_text_;
	Text draw_text_;
	int mouse_x_;
	int mouse_y_;
	int current_mouse_location_;
	bool bound_to_board_entities_;
	bool bound_to_restart_screen_;
	SDL_Texture *red_tex_;
	SDL_Texture *blue_tex_;
	SDL_Texture *red_arrow_;
	SDL_Texture *blue_arrow_;
	Board board_;
	std::vector<std::vector<Entity>> board_entities_;
	std::vector<Entity> display_arrows_;
	BasicButton restart_button_;
	BasicButton back_to_intro_button_;
	//when i saved the goofy file originally i made a typo, i won't fix it
	Entity gofy_;
#ifdef DEBUG_CONTROLS
	bool player_color_lock_;
#endif
};