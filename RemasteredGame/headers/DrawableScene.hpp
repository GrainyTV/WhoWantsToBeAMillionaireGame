#ifndef DRAWABLESCENE_HPP
#define DRAWABLESCENE_HPP

#include <stdexcept>
#include <future>
#include <string>
#include <filesystem>
#include "SDL.h"
#include "Hexa.hpp"

using std::string;
using std::future;
using std::async;
using std::launch;
using std::runtime_error;
using std::filesystem::path;

class DrawableScene
{
private:
public:
	virtual void Draw() const = 0;

	virtual unsigned int Hit(SDL_Point mousePos) = 0;

	virtual void Invoke(int index) const = 0;

	virtual ~DrawableScene() = default;

	SDL_Surface* CreateSurfaceFromFile(const string& text) const;

	SDL_Texture* CreateTextureFromFile(const string& text) const;

	SDL_Surface* CreateSurfaceFromText(const string& text, const string& type) const;

	SDL_Texture* CreateTextureFromText(const string& text, const string& type) const;

	SDL_Rect CenterTextInsideHexagon(const Hexa& hexagon, SDL_Texture* text) const;
};

#endif