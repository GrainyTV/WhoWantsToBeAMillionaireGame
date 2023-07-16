#include "Quad.hpp"

/**
 * 
 * Quad constructor creates a quad from 4 vertices given in "russian N" order (И).
 * @param vertexList : the given vertices
 * @param color : the color of the quad
 * 
 */
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

/**
 * 
 * Getter function for a singular vertex of the quad.
 * @param index : a provided index from 0 to 3
 * @return : the vertex at the given index
 * 
 */
Vec2 Quad::operator[](const int index) const
{
	if(index < 0 || index > 3)
	{
		throw invalid_argument(format("A sensible index for a quad is 0-3, you provided {0:d}.", index));
	}

	return Vec2(vertices[index].position.x, vertices[index].position.y);
}

/**
 * 
 * Render function for quads.
 * 
 */
void Quad::Draw() const
{
	SDL_RenderGeometry(renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
}