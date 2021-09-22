#pragma once
#include <SDL.h>
#include "GameState.hpp"
#include "Text.hpp"

class PlayingState : public GameState {
public:
	PlayingState();
	virtual void handleInput(Game &game, SDL_Event &event);
	virtual void update(Game &game);
private:
	Text text_;
};