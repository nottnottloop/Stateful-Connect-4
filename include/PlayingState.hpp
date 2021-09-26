#pragma once
#include <SDL.h>
#include <vector>
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
	void nextPlayerToMove(bool force = false);
	void updatePlayerMoveText();
	SDL_Texture *getArrowTex();
	void placeToken(int col);
	void checkWinOrDraw();
	void win(bool red_won);
	void draw();
	void resetGame();
	bool checkValidMouseLocation();
	int parseMouseLocation();
#ifdef DEBUG_CONTROLS
	void nearlyFillBoard();
#endif
private:
	bool won_;
	bool drawn_;
	bool player2_to_move_;
	Text player_to_move_text_;
	std::vector<Text> numbers_text_;
	Text win_text_;
	Text draw_text_;
	int mouse_x_;
	int mouse_y_;
	int current_mouse_location_;
	bool mouse_down_;
	SDL_Texture *red_tex_;
	SDL_Texture *blue_tex_;
	SDL_Texture *red_arrow_;
	SDL_Texture *blue_arrow_;
	std::vector<std::vector<Entity>> board_;
	std::vector<Entity> display_arrows_;
	std::vector<BasicButton> buttons_;
#ifdef DEBUG_CONTROLS
	bool player_color_lock_;
#endif
};