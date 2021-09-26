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
	void handleInput(Game &game, SDL_Event &event);
	void update(Game &game);
	void nextPlayerToMove();
	void updatePlayerMoveText();
private:
	bool player2_to_move_;
	Text player_to_move_text_;
	SDL_Texture *red_tex_;
	SDL_Texture *blue_tex_;
	std::vector<std::vector<Entity>> board_;
};