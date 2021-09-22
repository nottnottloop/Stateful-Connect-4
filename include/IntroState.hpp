#pragma once
#include <SDL.h>
#include "GameState.hpp"

class IntroState : public GameState {
public:
	virtual void handleInput(Game& game, SDL_Event& event);
	virtual void update(Game& game);
};