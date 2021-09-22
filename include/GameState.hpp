#pragma once
#include <SDL.h>

class Game;

class GameState {
public:
	virtual void handleInput(Game& game, SDL_Event& event) = 0;
	virtual void update(Game& game) = 0;
};