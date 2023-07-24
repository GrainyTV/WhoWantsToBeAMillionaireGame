#ifndef HEXA_HPP
#define HEXA_HPP

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <format>
#include <memory>
#include "Primitive.hpp"
#include "Triangle.hpp"
#include "Quad.hpp"

using std::vector;
using std::initializer_list;
using std::format;
using std::invalid_argument;
using std::data;
using std::unique_ptr;
using std::make_unique;

using std::shared_ptr;
using std::make_shared;

class Hexa : public Primitive
{
private:
	// Number of sides a hexagon should have
	static constexpr int SIDES = 6;

	// Default color of a hexagon
	const SDL_Color BLACK;

	// Border color of a hexagon
	const SDL_Color BLUE;

	// Hover color of a hexagon
	const SDL_Color ORANGE;

	// Correct answer color of a hexagon
	const SDL_Color GREEN;

	// Indicator wether hover is enabled
	bool overlayEnabled;

	// The 6 vertices of the hexagon
	vector<Vec2> nodes;

	// The individual parts of the hexagon (2x Triangle and 1x Quad)
	vector<shared_ptr<Primitive>> hexagonParts;

	// Same as the previous for the hover hexagon
	//vector<shared_ptr<Primitive>> hexagonOverlay;

	// The individual parts of the stroke of the hexagon
	vector<shared_ptr<Primitive>> hexagonStroke;

public:
	Hexa(const initializer_list<Vec2>& vertexList);

	Quad CalculateBorderQuad(const Vec2& v1, const Vec2& v2);

	void Draw() const override;

	bool Hit(SDL_Point& mousePos) const;

	vector<Vec2> _Vertices() const;

	bool operator==(const Hexa& hexa) const;

	bool operator<(const Hexa& hexa) const;

	Hexa Translate(int x, int y) const;

	int Width() const;

	int Height() const;

	void ChangeColor(SDL_Color color);

	void _Overlay(bool enabled);
};

#endif