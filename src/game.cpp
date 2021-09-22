#include <SDL.h>
#include "Game.hpp"

void Game::handleInput(SDL_Event& event) {
	state_->handleInput(*this, event);
}

void Game::update() {
	state_->update(*this);
}