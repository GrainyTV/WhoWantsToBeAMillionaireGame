#ifndef DRAWABLESCENE_HPP
#define DRAWABLESCENE_HPP

#include <stdexcept>
#include <future>
#include <string>
#include "SDL.h"
#include "Hexa.hpp"

using std::string;
using std::future;
using std::async;
using std::launch;
using std::runtime_error;

class DrawableScene
{
private:
public:
	virtual void Draw() const = 0;

	virtual ~DrawableScene() = default;

	SDL_Surface* CreateSurfaceFromText(const string& text, const string& type) const;

	SDL_Texture* CreateTextureFromText(const string& text, const string& type) const;

	SDL_Rect CenterTextInsideHexagon(const Hexa& hexagon, SDL_Texture* text) const;
};

#endif