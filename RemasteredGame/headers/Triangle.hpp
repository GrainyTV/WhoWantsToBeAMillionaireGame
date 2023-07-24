#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <initializer_list>
#include <vector>
#include <stdexcept>
#include <format>
#include "Primitive.hpp"

using std::initializer_list;
using std::vector;
using std::invalid_argument;
using std::format;

class Triangle : public Primitive
{
private:
	// Number of sides a triangle should have is 3
	static constexpr int SIDES = 3;

	// A 3 elements long array for the vertices of the triangle
	SDL_Vertex vertices[SIDES];

public:
	Triangle(const initializer_list<Vec2>& vertexList, const SDL_Color& color);

	void Draw() const override;

	bool Hit(int x, int y) const;

	void ChangeColor(SDL_Color color);

	static double TriangleArea(Vec2 x1, Vec2 x2, Vec2 x3);
};

#endif