#include <SDL.h>
#include "Game.hpp"
#include "GameState.hpp"

Game::Game() {
	quit_ = false;
}

void Game::handleInput(SDL_Event& event) {
	state_->handleInput(*this, event);
}

void Game::update() {
	state_->update(*this);
}

void Game::setState(GameState* state) {
	state_ = state;
}

GameState* Game::getState(GameState* state) {
	return state_;
}