#include <iostream>
#include <SDL.h>
#include "PlayingState.hpp"
#include "Text.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"

extern RenderWindow window;

PlayingState::PlayingState()
:player_to_move_text_({768, 0}, {0, 0}, 50), player2_to_move_(false) {
	state_name_ = "PlayingState";
	player_to_move_text_.openFont("res/fixedsys.ttf", 50);
	updatePlayerMoveText();
}

void PlayingState::nextPlayerToMove() {
	player2_to_move_ = !player2_to_move_;
	updatePlayerMoveText();
}

void PlayingState::updatePlayerMoveText() {
	if (player2_to_move_) {
		player_to_move_text_.loadFontTexture({0, 255, 0}, "Player 2");
	} else {
		player_to_move_text_.loadFontTexture({255, 0, 0}, "Player 1");
	}
}

void PlayingState::handleInput(Game& game, SDL_Event& event) {
		switch (event.type) {
			case SDL_QUIT:
				game.quit_ = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_SPACE:
						nextPlayerToMove();
						break;
				}
		}
}

void PlayingState::update(Game& game) {
	window.clear(255, 255, 255, 0xFF);
	window.render(player_to_move_text_);
	window.display();
	window.showWindow();
}