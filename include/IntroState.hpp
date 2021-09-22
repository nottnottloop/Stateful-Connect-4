#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "GameState.hpp"
#include "Text.hpp"

class IntroState : public GameState {
public:
	IntroState();
	virtual void handleInput(Game& game, SDL_Event& event);
	virtual void update(Game& game);
private:
	Text text_;
};