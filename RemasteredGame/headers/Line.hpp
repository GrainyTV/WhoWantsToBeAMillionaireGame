#ifndef LINE_HPP
#define LINE_HPP

#include <stdexcept>
#include <format>
#include "SDL.h"
#include "Primitive.hpp"

using std::invalid_argument;
using std::format;

class Line : public Primitive
{
private:
	SDL_Color color;

	SDL_Rect line;

public:
	Line(Vec2 x0, Vec2 x1, int width, SDL_Color color);

	void Draw() const override;
};

#endif