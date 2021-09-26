#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "GameState.hpp"
#include "Text.hpp"

class IntroState : public GameState {
public:
	IntroState();
	void handleInput(Game &game, const SDL_Event &event);
	void update(Game &game);
private:
	Text text_;
};