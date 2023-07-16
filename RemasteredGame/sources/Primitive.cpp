#include "Primitive.hpp"
#include "Window.hpp"

/**
 * 
 * To normalize a vector. We use this with the hexagon stroke calculation.
 * 
 */
void Vec2::Normalize()
{
	double distance = sqrt(pow(x, 2) + pow(y, 2));
	x /= distance;
	y /= distance;
}

/**
 * 
 * Equality operator for 2D vectors.
 * @param vector : the vector we compare with this one
 * @return : either true if they are equal or false
 * 
 */
bool Vec2::operator==(const Vec2& vector)
{
	if(vector.x == x && vector.y == y)
	{
		return true;
	}

	return false;
}

/**
 * 
 * Print method for Vec2. Only for debugging purposes.
 * 
 */
void Vec2::Print() const
{
	printf("(%.2f, %.2f)\n", x, y);
}

/**
 * 
 * Primitive constructor sets renderer so we don't need to do it in every single derived class.
 * 
 */
Primitive::Primitive() : renderer(Window::_Renderer())
{
}