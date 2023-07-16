#include "Line.hpp"
#include "Window.hpp"

Line::Line(Vec2 x0, Vec2 x1, int width, SDL_Color color) : color(color)
{
	if(x0.x != x1.x && x0.y != x1.y)
	{
		throw invalid_argument(format("The provided line is neither horizontal nor vertical.\n -> x0({0},{1}), x1({2},{3})", x0.x, x0.y, x1.x, x1.y));
	}

	if(x0.x == x1.x && x0.y == x1.y)
	{
		throw invalid_argument(format("You are trying to create a line from the same two vertices.\n({0},{1})", x0.x, x0.y));
	}

	Vec2 diagonallyOppositeVertices[2];

	if(x1.x - x0.x == 0)
	{
		diagonallyOppositeVertices[0] = Vec2(x0.x - (width - 1) / 2, x0.y);
		diagonallyOppositeVertices[1] = Vec2(x1.x + (width - 1) / 2, x1.y);
	}
	else
	{
		diagonallyOppositeVertices[0] = Vec2(x0.x, x0.y - (width - 1) / 2);
		diagonallyOppositeVertices[1] = Vec2(x1.x, x1.y + (width - 1) / 2);
	}

	line.x = diagonallyOppositeVertices[0].x;
	line.y = diagonallyOppositeVertices[0].y;
	line.w = (diagonallyOppositeVertices[1].x - diagonallyOppositeVertices[0].x < 0)
		? diagonallyOppositeVertices[0].x - diagonallyOppositeVertices[1].x
		: diagonallyOppositeVertices[1].x - diagonallyOppositeVertices[0].x;
	line.h = (diagonallyOppositeVertices[1].y - diagonallyOppositeVertices[0].y < 0)
		? diagonallyOppositeVertices[0].y - diagonallyOppositeVertices[1].y
		: diagonallyOppositeVertices[1].y - diagonallyOppositeVertices[0].y;
}

void Line::Draw() const
{
	SDL_SetRenderDrawColor(Window::_Renderer(), color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(Window::_Renderer(), &line);
}