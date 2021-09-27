#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <tuple>
#include "Text.hpp"
#include "BasicButton.hpp"

class RenderWindow 
{
public:
	RenderWindow(const char* title, int w, int h);
	SDL_Texture* loadTexture(const char* file_path);
	void cleanUp();
	void clear();
	void clear(int r, int g, int b, int a);
	void clear(SDL_Color, int a);
	void changeColor(SDL_Color color);
	void render(Text text, bool centered=false) const;
	void render(std::tuple<SDL_Rect, SDL_Rect> rects, SDL_Texture* tex) const;
	void render(const BasicButton &button);
	void renderLine(int x1, int y1, int x2, int y2, SDL_Color color);
	void renderRect(SDL_Rect rect, SDL_Color color);
	void renderBlendedRect(SDL_Rect rect, SDL_Color color);
	void display() const;
	void showWindow();
	void renderRect(int r, int g, int b);
	SDL_Renderer* renderer_;
private:
	SDL_Window* window_;
};