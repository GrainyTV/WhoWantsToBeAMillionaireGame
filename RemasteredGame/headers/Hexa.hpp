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
	static constexpr int SIDES = 6;

	const SDL_Color BLACK;

	const SDL_Color BLUE;

	const SDL_Color ORANGE;

	bool overlayEnabled;

	vector<Vec2> nodes;

	vector<shared_ptr<Primitive>> hexagonParts;

	vector<shared_ptr<Primitive>> hexagonOverlay;

	vector<shared_ptr<Primitive>> hexagonStroke;

	//vector<unique_ptr<Primitive>> hexagonParts;

	//vector<unique_ptr<Primitive>> hexagonStroke;

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

	void _Overlay(bool enabled);
};

#endif