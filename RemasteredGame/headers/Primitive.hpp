#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "SDL.h"

struct Vec2
{
	double x;
	
	double y;

	void Normalize();

	bool operator==(const Vec2& vector);

	void Print() const;
};

class Primitive
{
protected:
	SDL_Renderer* renderer;

	Primitive();

public:
	virtual void Draw() const = 0;

	virtual void ChangeColor(SDL_Color color) = 0;

	virtual ~Primitive() = default;
};

#endif