#include <iostream>
#include "GameState.hpp"

GameState::GameState()
:initialised_(false) {
}

GameState::GameState(std::string state_name)
:initialised_(false),
state_name_(state_name) {
}