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
flavor_text_({SCREEN_WIDTH / 2 + 50, 125}, {0, 0}),
//140 difference between bottommost button of a group and next text object
//difference of 50 in height between text and buttons in a block
two_player_text_({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200}, {0, 0}),
two_player_button_(BasicButton({SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50 - 100, 200, 100}, {0, 0}, BLACK, PEACH, BLACK, 5, "Two Player")),
ai_text_({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 10}, {0, 0}),
can_beat_button_(BasicButton({SCREEN_WIDTH / 2 - 225, SCREEN_HEIGHT / 2 + 40, 200, 100}, {0, 0}, BLACK, LIME_GREEN, BLACK, 5, "Can beat")),
cant_beat_button_(BasicButton({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 40, 200, 100}, {0, 0}, BLACK, RED, BLACK, 5, "Can't beat")),
internet_play_text_({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 180}, {0, 0}),
internet_play_button_(BasicButton({SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 230, 400, 100}, {0, 0}, BLACK, MODS_BLUE, BLACK, 5, "Internet Play"))
{
	text_.openFont("res/curly.ttf", 125);
	flavor_text_.openFont("res/curly.ttf", 40);
	two_player_text_.openFont("res/fixedsys.ttf", 40);
	ai_text_.openFont("res/fixedsys.ttf", 40);
	internet_play_text_.openFont("res/fixedsys.ttf", 40);
	text_.loadFontTexture(BLACK, "Stateful Connect Four", true);
	flavor_text_.loadFontTexture(BLACK, "\"I guarantee you cannot find a higher quality Connect Four game in existence\"\n                                                                                                                 NottNott", true, true, SCREEN_WIDTH);
	two_player_text_.loadFontTexture(BLACK, "Local Play");
	ai_text_.loadFontTexture(BLACK, "Play against AI that you:");
	internet_play_text_.loadFontTexture(BLACK, "Internet Play");
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
					if (can_beat_button_.clicked(mouse_x_, mouse_y_)) {
						const Uint8 *state = SDL_GetKeyboardState(nullptr);
						bool goofy_ai = false;
						bool turbo_ai = false;
						SDL_Color color = HONEYDEW;
						if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {
							goofy_ai = true;
							color = LIGHT_PURPLE;
						}
						if (state[SDL_SCANCODE_RCTRL] || state[SDL_SCANCODE_LCTRL]) {
							turbo_ai = true;
						}
						game.playingState(game_mode::ONE_PLAYER, false, goofy_ai, turbo_ai, color);
					}
					if (cant_beat_button_.clicked(mouse_x_, mouse_y_)) {
						game.playingState(game_mode::ONE_PLAYER, true, false, false, GAINSBORO);
					}
					if (two_player_button_.clicked(mouse_x_, mouse_y_)) {
						game.playingState(game_mode::TWO_PLAYER, false);
					}
					if (internet_play_button_.clicked(mouse_x_, mouse_y_)) {
						game.playingState(game_mode::ONLINE, false);
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
	window.render(two_player_text_, true);
	window.render(ai_text_, true);
	window.render(two_player_button_);
	window.render(can_beat_button_);
	window.render(cant_beat_button_);
	window.render(internet_play_text_, true);
	window.render(internet_play_button_);
	window.display();
	window.showWindow();
}