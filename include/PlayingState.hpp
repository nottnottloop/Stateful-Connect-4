#pragma once
#include <SDL.h>
#include "GameState.hpp"

class Game;

class PlayingState : public GameState {
public:
	virtual void handleInput(Game& game, SDL_Event& event);
	virtual void update(Game& game);
};