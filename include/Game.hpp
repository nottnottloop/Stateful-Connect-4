#pragma once
#include <SDL.h>
#include "GameState.hpp"
#include "IntroState.hpp"
#include "PlayingState.hpp"
#include "Constants.hpp"

class Game {
public:
	Game();
	void handleInput(SDL_Event &event);
	void update();

	void setState(GameState *state);
	void introState();
	void playingState(game_mode mode, bool minimax, bool goofy=false, bool turbo=false, SDL_Color color=BUBBLE);
	GameState *getState(GameState *state);

	bool quit_;
private:
	GameState *state_;
	IntroState *introstate_;
	PlayingState *playingstate_;
};