#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "GameState.hpp"
#include "Text.hpp"
#include "BasicButton.hpp"

class IntroState : public GameState {
public:
	IntroState();
	void handleInput(Game &game, const SDL_Event &event);
	void update(Game &game);
private:
	Text text_;
	Text flavor_text_;
	BasicButton start_game_button_;
	bool mouse_down_;
	int mouse_x_;
	int mouse_y_;
};