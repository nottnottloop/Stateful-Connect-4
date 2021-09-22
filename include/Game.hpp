#pragma once
#include <SDL.h>
#include "GameState.hpp"

class Game {
public:
	virtual void handleInput(SDL_Event& event);
	virtual void update();
private:
	GameState* state_;
};