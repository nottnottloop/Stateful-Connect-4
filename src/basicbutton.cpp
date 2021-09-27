#include <SDL.h>
#include "Text.hpp"
#include "Math.hpp"
#include "BasicButton.hpp"
#include "Constants.hpp"

BasicButton::BasicButton(SDL_Rect button_rect, Vector2f offset, SDL_Color background_color, SDL_Color border_color, int border_thickness, char *message)
:offset_(offset), rect_(button_rect), text_(Vector2f{button_rect.x + button_rect.w / 2.0f, button_rect.y + button_rect.h / 2.0f}, Vector2f{0, 0}), background_color_(background_color), border_color_(border_color), border_thickness_(border_thickness), visible_(true) {
	text_.openFont("res/fixedsys.ttf", 35);
	text_.loadFontTexture(BLACK, message);
	int tex_w, tex_h;
	SDL_QueryTexture(text_.getFgTex(), nullptr, nullptr, &tex_w, &tex_h);
	text_.setPos({button_rect.x + button_rect.w / 2 - tex_w / 2.0f, button_rect.y + button_rect.h / 2 - tex_h / 2.0f});
}

bool BasicButton::clicked(Uint32 x, Uint32 y) {
	if (x > rect_.x && x < rect_.x + rect_.w) {
		if (y > rect_.y && y < rect_.y + rect_.h) {
			return true;
		}
	}
	return false;
}