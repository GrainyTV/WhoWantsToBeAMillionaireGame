#include "Triangle.hpp"

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

void Triangle::Draw() const
{
	SDL_RenderGeometry(renderer, NULL, &vertices[0], SIDES, NULL, 0);
}