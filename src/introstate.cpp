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
text_({SCREEN_WIDTH / 2, 0}, {0, 0}),
flavor_text_({SCREEN_WIDTH / 2 + 50, 200}, {0, 0}),
local_play_text_({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50}, {0, 0}),
one_player_button_(BasicButton({SCREEN_WIDTH / 2 - 225, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, BLACK, LIME_GREEN, BLACK, 5, "One Player")),
two_player_button_(BasicButton({SCREEN_WIDTH / 2 + 25, SCREEN_HEIGHT / 2, 200, 100}, {0, 0}, BLACK, PEACH, BLACK, 5, "Two Player")),
internet_play_text_({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150}, {0, 0}),
internet_play_button_(BasicButton({SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 200, 400, 100}, {0, 0}, BLACK, MODS_BLUE, BLACK, 5, "Internet Play"))
{
	text_.openFont("res/curly.ttf", 125);
	flavor_text_.openFont("res/curly.ttf", 40);
	local_play_text_.openFont("res/fixedsys.ttf", 40);
	internet_play_text_.openFont("res/fixedsys.ttf", 40);
	text_.loadFontTexture({0, 0, 0}, "Stateful Connect Four", true);
	flavor_text_.loadFontTexture({0, 0, 0}, "\"I guarantee you cannot find a higher quality Connect Four game in existence\"\n                                                                                                                 NottNott", true, true, SCREEN_WIDTH);
	local_play_text_.loadFontTexture({0, 0, 0}, "Local Play");
	internet_play_text_.loadFontTexture({0, 0, 0}, "Internet Play");
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
			case SDL_KEYDOWN:
			case SDL_MOUSEBUTTONUP:
				if (mouse_down_) {
					if (one_player_button_.clicked(mouse_x_, mouse_y_)) {
						const Uint8 *state = SDL_GetKeyboardState(nullptr);
						bool goofy_ai = false;
						bool turbo_ai = false;
						if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {
							goofy_ai = true;
							printf("goofy");
						}
						if (state[SDL_SCANCODE_RCTRL] || state[SDL_SCANCODE_LCTRL]) {
							turbo_ai = true;
							printf("turbo");
						}
						game.playingState(game_mode::ONE_PLAYER, goofy_ai, turbo_ai);
					}
					if (two_player_button_.clicked(mouse_x_, mouse_y_)) {
						game.playingState(game_mode::TWO_PLAYER);
					}
					if (internet_play_button_.clicked(mouse_x_, mouse_y_)) {
						game.playingState(game_mode::ONLINE);
					}
				}
				mouse_down_ = false;
				break;
		}
}

void IntroState::update(Game &game) {
	window.clear(255, 255, 255, 0xFF);
	window.render(text_, true);
	window.render(flavor_text_, true);
	window.render(local_play_text_, true);
	window.render(one_player_button_);
	window.render(two_player_button_);
	window.render(internet_play_text_, true);
	window.render(internet_play_button_);
	window.display();
	window.showWindow();
}