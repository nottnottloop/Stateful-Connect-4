#include <SDL.h>
#include <SDL_ttf.h>
#include "Text.hpp"
#include "Math.hpp"
#include "RenderWindow.hpp"

extern RenderWindow window;

Text::Text(Vector2f pos, Vector2f offset)
: Entity(pos, offset, {0, 0, 0, 0}, {0, 0, 0, 0}, nullptr, nullptr) {
}

void Text::loadFontTexture(SDL_Color color, const char* text, bool blended, bool wrapped, int wrap_length) {
	SDL_DestroyTexture(fg_tex_);
	SDL_Surface *surface;
	if (!blended) {
		surface = TTF_RenderText_Solid(font_, text, color);
	} else if (!wrapped) {
		surface = TTF_RenderText_Blended(font_, text, color);
	} else if (wrapped && wrap_length != 0) {
		surface = TTF_RenderText_Blended_Wrapped(font_, text, color, wrap_length);
	}
	fg_tex_ = SDL_CreateTextureFromSurface(window.renderer_, surface);
	SDL_FreeSurface(surface);
}

SDL_Rect Text::renderTextInfo() {
	int tex_w = 0;
	int tex_h = 0;
	SDL_QueryTexture(fg_tex_, nullptr, nullptr, &tex_w, &tex_h);
	SDL_Rect dst_rect = { getPos().x, getPos().y, tex_w, tex_h };
	return dst_rect;
}

SDL_Rect Text::renderCenteredTextInfo() {
	int tex_w = 0;
	int tex_h = 0;
	SDL_QueryTexture(fg_tex_, nullptr, nullptr, &tex_w, &tex_h);
	SDL_Rect dst_rect = { getPos().x - tex_w / 2, getPos().y, tex_w, tex_h };
	return dst_rect;
}

void Text::openFont(const char* file_path, int size) {
	font_ = TTF_OpenFont(file_path, size);
}