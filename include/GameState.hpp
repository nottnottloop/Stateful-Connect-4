#pragma once
#include <SDL.h>

class Game;

class GameState {
public:
	GameState();
	virtual void handleInput(Game& game, SDL_Event& event) = 0;
	virtual void update(Game& game) = 0;
protected:
	bool initialised_;
};