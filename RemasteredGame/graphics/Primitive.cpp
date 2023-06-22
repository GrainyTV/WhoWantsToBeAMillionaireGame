#include "Primitive.hpp"
#include "Window.hpp"

void Vec2::Normalize()
{
	double distance = sqrt(pow(x, 2) + pow(y, 2));
	x /= distance;
	y /= distance;
}

void Vec2::Print() const
{
	printf("(%.2f, %.2f)\n", x, y);
}

Primitive::Primitive() : renderer(Window::_Renderer())
{
}