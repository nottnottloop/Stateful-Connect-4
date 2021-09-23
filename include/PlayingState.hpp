#pragma once
#include <SDL.h>
#include "GameState.hpp"
#include "Text.hpp"

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
};