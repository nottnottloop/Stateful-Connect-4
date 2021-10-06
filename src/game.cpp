#include <SDL.h>
#include "Game.hpp"
#include "GameState.hpp"
#include "IntroState.hpp"
#include "PlayingState.hpp"
#include "Constants.hpp"

Game::Game() {
	quit_ = false;
	introstate_ = new IntroState;
	playingstate_ = new PlayingState;
	introState();
}

void Game::handleInput(SDL_Event &event) {
	state_->handleInput(*this, event);
}

void Game::update() {
	state_->update(*this);
}

void Game::setState(GameState *state) {
	state_ = state;
}

//set the state to intro
void Game::introState() {
	state_ = introstate_;
}

//set the state to playing with a given game mode
void Game::playingState(game_mode mode, bool minimax, bool goofy, bool turbo, SDL_Color color) {
	playingstate_->setGameMode(mode);
	playingstate_->setMinimaxAi(minimax);
	playingstate_->setGoofyAi(goofy);
	playingstate_->setTurboAi(turbo);
	playingstate_->changeColor(color);
	playingstate_->updatePlayerMoveText();
	state_ = playingstate_;
}

GameState* Game::getState(GameState *state) {
	return state_;
}