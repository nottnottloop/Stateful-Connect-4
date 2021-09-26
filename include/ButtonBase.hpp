#pragma once
#include <SDL.h>
#include "Math.hpp"

class ButtonBase {
public:
	virtual bool clicked(Uint32 x, Uint32 y) = 0;
};