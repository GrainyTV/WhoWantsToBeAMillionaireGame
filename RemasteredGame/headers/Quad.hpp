#ifndef QUAD_HPP
#define QUAD_HPP

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <format>
#include "Primitive.hpp"

using std::vector;
using std::initializer_list;
using std::invalid_argument;
using std::format;

class Quad : public Primitive
{
private:
	static constexpr int SIDES = 4;

	vector<SDL_Vertex> vertices;

public:
	Quad(const initializer_list<Vec2>& vertexList, const SDL_Color& color);

	Vec2 operator[](const int index) const;

	void Draw() const override;
};

#endif