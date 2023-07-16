#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "SDL.h"

struct Vec2
{
	// X coordinate of a 2D vector
	double x;
	
	// Y coordinate of a 2D vector
	double y;

	void Normalize();

	bool operator==(const Vec2& vector);

	void Print() const;
};

class Primitive
{
protected:
	// Distributed renderer for derived primitive classes
	SDL_Renderer* renderer;

	Primitive();

public:
	virtual void Draw() const = 0;

	virtual ~Primitive() = default;
};

#endif