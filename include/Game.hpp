#pragma once
#include <SDL.h>
#include "GameState.hpp"
#include "Constants.hpp"

class Game {
public:
	Game();
	void handleInput(SDL_Event &event);
	void update();

	void setState(GameState *state);
	void introState();
	void playingState(game_mode mode);
	GameState *getState(GameState *state);

	bool quit_;
private:
	GameState *state_;
	GameState *introstate_;
	GameState *playingstate_;
	game_mode game_mode_;
};