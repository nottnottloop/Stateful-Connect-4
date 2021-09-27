#include <iostream>
#include <SDL.h>
#include "IntroState.hpp"
#include "Text.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "BasicButton.hpp"
#include "Constants.hpp"
#include "Math.hpp"

extern RenderWindow window;

IntroState::IntroState()
:text_({0, 0}, {0, 0}),
start_game_button_(BasicButton({SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, CYAN, PURPLE, 5, "Start Game")) {
	state_name_ = "IntroState";
	text_.openFont("res/fixedsys.ttf", 50);
	text_.loadFontTexture({0, 0, 0}, state_name_.c_str());
}

void IntroState::handleInput(Game &game, const SDL_Event &event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
		}
}

void IntroState::update(Game &game) {
	window.clear(255, 255, 255, 0xFF);
	window.render(text_);
	window.render(start_game_button_);
	window.display();
	window.showWindow();
}