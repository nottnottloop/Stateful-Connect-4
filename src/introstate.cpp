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
:GameState("IntroState"),
mouse_down_(false),
text_({0, 0}, {0, 0}),
flavor_text_({0, 200}, {0, 0}),
start_game_button_(BasicButton({SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, BUBBLE, BLACK, 5, "Start Game")) {
	text_.openFont("res/curly.ttf", 125);
	flavor_text_.openFont("res/curly.ttf", 40);
	text_.loadFontTexture({0, 0, 0}, "Stateful Connect Four", true);
	flavor_text_.loadFontTexture({0, 0, 0}, "\"I guarantee that you cannot find a higher quality Connect Four game in existence.\"\n                                                                                                                 NottNott", true, true, SCREEN_WIDTH);
}

void IntroState::handleInput(Game &game, const SDL_Event &event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&mouse_x_, &mouse_y_);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse_down_ = true;
				break;
			case SDL_MOUSEBUTTONUP:
				if (start_game_button_.clicked(mouse_x_, mouse_y_) && mouse_down_) {
					game.playingState();
				}
				mouse_down_ = false;
				break;
		}
}

void IntroState::update(Game &game) {
	window.clear(255, 255, 255, 0xFF);
	window.render(text_);
	window.render(flavor_text_);
	window.render(start_game_button_);
	window.display();
	window.showWindow();
}