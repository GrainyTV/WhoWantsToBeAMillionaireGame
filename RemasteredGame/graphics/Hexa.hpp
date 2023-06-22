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

class Hexa : public Primitive
{
private:
	static constexpr int SIDES = 6;

	vector<unique_ptr<Primitive*>> hexagonParts;

	vector<unique_ptr<Primitive*>> hexagonStroke;

public:
	Hexa(const initializer_list<Vec2>& vertexList);

	Quad CalculateBorderQuad(const Vec2& v1, const Vec2& v2);

	void Draw() const override;
};

#endif