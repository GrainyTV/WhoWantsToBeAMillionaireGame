#include "Quad.hpp"

Quad::Quad(const initializer_list<Vec2>& vertexList, const SDL_Color& color) : Primitive()
{
	if(vertexList.size() != SIDES)
	{
		throw invalid_argument(format("A quad should have 4 vertices. Not less, not more. The one you provided has {0:d}.", vertexList.size()));
	}

	for(auto vertex : vertexList)
	{
		vertices.push_back((SDL_Vertex) { SDL_FPoint(vertex.x, vertex.y), SDL_Color(color.r, color.g, color.b, color.a), SDL_FPoint(0) });
	}

	vertices.push_back(vertices[1]);
	vertices.push_back(vertices[2]);
}

Vec2 Quad::operator[](const int index) const
{
	if(index < 0 || index > 3)
	{
		throw invalid_argument(format("A sensible index for a quad is 0-3, you provided {0:d}.", index));
	}

	return Vec2(vertices[index].position.x, vertices[index].position.y);
}

void Quad::Draw() const
{
	SDL_RenderGeometry(renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
}