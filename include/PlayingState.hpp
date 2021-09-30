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
	PlayingState();
	void handleInput(Game &game, const SDL_Event &event);
	void update(Game &game);
	void setGameMode(game_mode mode);
	void setGoofyAi(bool goofy);
	void setTurboAi(bool turbo);
	void cycleColor(bool backward);
	void randomPlayerToMove();
	void nextPlayerToMove(bool force = false);
	void updatePlayerMoveText();
	SDL_Texture *getArrowTex();
	void tryToPlaceToken(int col);
	void aiMove();
	bool isValidColumn(int col);
	void placeToken(int col, std::array<std::array<int, NUM_COLS>, NUM_ROWS> &board, bool real);
	void postTokenUpdate();
	void win(bool red_won);
	void draw();
	void resetGame();
	bool checkValidMouseLocation();
	int parseMouseLocation();
#ifdef DEBUG_CONTROLS
	void nearlyFillBoard();
#endif
private:
	std::mt19937_64 rd_;
	game_mode game_mode_;
	bool goofy_ai_;
	bool turbo_ai_;
	int next_ai_move_;
	int color_index_location_;
	std::vector<SDL_Color> colors_;
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
	std::array<std::array<int, NUM_COLS>, NUM_ROWS> board_;
	std::vector<std::vector<Entity>> board_entities_;
	std::vector<Entity> display_arrows_;
	BasicButton restart_button_;
	BasicButton back_to_intro_button_;
#ifdef DEBUG_CONTROLS
	bool player_color_lock_;
#endif
};