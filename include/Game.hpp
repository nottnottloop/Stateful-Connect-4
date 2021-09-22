#pragma once
#include <SDL.h>
#include "GameState.hpp"

class Game {
public:
	Game();
	void handleInput(SDL_Event& event);
	void update();
	void setState(GameState* state);
	GameState* getState(GameState* state);

	bool quit_;
private:
	GameState* state_;
};