#pragma once
#include <SDL.h>
#include "Text.hpp"
#include "ButtonBase.hpp"

class BasicButton : public ButtonBase {
public:
	BasicButton(SDL_Rect button_rect, Vector2f offset, SDL_Color text_color, SDL_Color background_color, SDL_Color border_color, int border_thickness, char *message);
	bool clicked(Uint32 x, Uint32 y);
private:
	SDL_Rect rect_;
	Vector2f offset_;
	Text text_;
	SDL_Color text_color_;
	SDL_Color background_color_;
	SDL_Color border_color_;
	int border_thickness_;
	bool visible_;
	friend class RenderWindow;
};