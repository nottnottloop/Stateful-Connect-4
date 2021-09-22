#include <SDL.h>
#include "IntroState.hpp"
#include "Text.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"

extern RenderWindow window;

void IntroState::handleInput(Game& game, SDL_Event& event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
		}
}

void IntroState::update(Game& game) {
	window.display();
	window.clear(255, 255, 255, 0xFF);
	window.showWindow();
}