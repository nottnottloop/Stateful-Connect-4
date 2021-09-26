#pragma once
#include <SDL.h>
#include <vector>
#include "GameState.hpp"
#include "Text.hpp"
#include "Entity.hpp"
#include "Constants.hpp"

class PlayingState : public GameState {
public:
	PlayingState();
	void handleInput(Game &game, const SDL_Event &event);
	void update(Game &game);
	void nextPlayerToMove();
	void updatePlayerMoveText();
	SDL_Texture *getArrowTex();
	void placeToken(int col);
	bool checkValidMouseLocation();
	int parseMouseLocation();
private:
	bool player2_to_move_;
	Text player_to_move_text_;
	std::vector<Text> numbers_text_;
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
};