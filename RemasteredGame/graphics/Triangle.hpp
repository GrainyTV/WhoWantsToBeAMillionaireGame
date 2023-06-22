#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <format>
#include "SDL.h"
#include "Primitive.hpp"

using std::initializer_list;
using std::vector;
using std::invalid_argument;
using std::format;

class Triangle : public Primitive
{
private:
	static constexpr int SIDES = 3;

	SDL_Vertex vertices[SIDES];

public:
	Triangle(const initializer_list<Vec2>& vertexList, const SDL_Color& color);

	void Draw() const override;
};

#endif