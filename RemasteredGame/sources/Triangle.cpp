#include "Triangle.hpp"

/**
 * 
 * Triangle constructor creates a triangle from 3 vertices and a color.
 * 
 */
Triangle::Triangle(const initializer_list<Vec2>& vertexList, const SDL_Color& color) : Primitive()
{
	if(vertexList.size() != SIDES)
	{
		throw invalid_argument(format("A triangle should have 3 vertices. Not less, not more. The one you provided has {0:d}.", vertexList.size()));
	}

	const vector<Vec2> indexableVertexList(vertexList);

	for(int i = 0; i < SIDES; ++i)
	{	
		vertices[i] = (SDL_Vertex) { SDL_FPoint(indexableVertexList[i].x, indexableVertexList[i].y), SDL_Color(color.r, color.g, color.b, color.a), SDL_FPoint(0) };
	}
}

/**
 * 
 * Render function for triangles.
 * 
 */
void Triangle::Draw() const
{
	SDL_RenderGeometry(renderer, NULL, &vertices[0], SIDES, NULL, 0);
}

/**
 * 
 * Intersection function for triangles using area calculation of 3 smaller triangles.
 * @param x : the x coordinate to check for
 * @param y : the y coordinate to check for
 * @return : either true if coordinate pair is inside the triangle or false if it is outside
 * 
 */
bool Triangle::Hit(int x, int y) const
{
	/* Calculate area of triangle ABC */
	double A = TriangleArea(Vec2(vertices[0].position.x, vertices[0].position.y), Vec2(vertices[1].position.x, vertices[1].position.y), Vec2(vertices[2].position.x, vertices[2].position.y));

	/* Calculate area of triangle PBC */
	double A1 = TriangleArea(Vec2(x, y), Vec2(vertices[1].position.x, vertices[1].position.y), Vec2(vertices[2].position.x, vertices[2].position.y));

	/* Calculate area of triangle PAC */
	double A2 = TriangleArea(Vec2(vertices[0].position.x, vertices[0].position.y), Vec2(x, y), Vec2(vertices[2].position.x, vertices[2].position.y));

	/* Calculate area of triangle PAB */
	double A3 = TriangleArea(Vec2(vertices[0].position.x, vertices[0].position.y), Vec2(vertices[1].position.x, vertices[1].position.y), Vec2(x, y));

	return (A == A1 + A2 + A3);
}

void Triangle::ChangeColor(SDL_Color color)
{
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
}

/**
 * 
 * A utility function to calculate area of triangle formed by (x1, y1), (x2, y2) and (x3, y3)
 * param x1 : the location of the first vertex
 * param x2 : the location of the second vertex
 * param x3 : the location of the third vertex
 * return : the area of the triangle
 *  
 */
double Triangle::TriangleArea(Vec2 x1, Vec2 x2, Vec2 x3)
{
	return abs((x1.x * (x2.y - x3.y) + x2.x * (x3.y - x1.y) + x3.x * (x1.y - x2.y)) / 2.0);
}