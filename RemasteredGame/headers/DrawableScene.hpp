#ifndef DRAWABLESCENE_HPP
#define DRAWABLESCENE_HPP

#include <stdexcept>
#include <future>
#include <string>
#include <filesystem>
#include <tuple>
#include <deque>
#include <functional>
#include "SDL.h"
#include "Hexa.hpp"
#include "Line.hpp"

using std::string;
using std::future;
using std::async;
using std::launch;
using std::runtime_error;
using std::filesystem::path;
using std::deque;
using std::tuple;
using std::function;
using std::get;

class DrawableScene
{
private:
protected:
	// Our clickable buttons
	// They consist of a hexagon, a texture of text and a backend function call
	deque<tuple<Hexa, SDL_Texture*, function<void()>>> buttons;

	// Lines under each of the buttons
	deque<Line> buttonLines;

	// Width of the lines
	static constexpr int lineWidth { 5 };

	// Color of the lines
	static constexpr SDL_Color BLUE { SDL_Color(95, 194, 253, 255) };

	// Background image
	SDL_Texture* background;

	// Wether to ignore events invoked by the mouse
	bool mouseEnabled { true };

public:
	virtual void Draw() const = 0;

	virtual ~DrawableScene() = default;

	bool _MouseEnabled() const;

	void _MouseEnabled(bool enabled);

	unsigned int Hit(SDL_Point mousePos);

	void Invoke(int index) const;

	SDL_Surface* CreateSurfaceFromFile(const string& text) const;

	SDL_Texture* CreateTextureFromFile(const string& text) const;

	SDL_Surface* CreateSurfaceFromText(const string& text, const string& type) const;

	SDL_Texture* CreateTextureFromText(const string& text, const string& type) const;

	SDL_Rect CenterTextInsideHexagon(const Hexa& hexagon, SDL_Texture* text) const;
};

#endif