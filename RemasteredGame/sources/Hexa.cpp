#include "Hexa.hpp"

Hexa::Hexa(const initializer_list<Vec2>& vertexList)
:
Primitive(),
BLACK(SDL_Color(0, 0, 0, 255)),
BLUE(SDL_Color(95, 194, 253, 255)),
ORANGE(SDL_Color(254, 125, 5, 128)),
overlayEnabled(false)
{
	if(vertexList.size() != SIDES)
	{
		throw invalid_argument(format("A hexagon should have 6 vertices. Not less, not more. The one you provided has {0:d}.", vertexList.size()));
	}

	for(auto vertex : vertexList)
	{
		nodes.push_back(vertex);
	}

	const vector<Vec2> vertices(vertexList);
	
	/*hexagonParts.push_back(unique_ptr<Triangle>(new Triangle({ vertices[0], vertices[1], vertices[2] }, BLACK)));
	hexagonParts.push_back(unique_ptr<Triangle>(new Triangle({ vertices[3], vertices[4], vertices[5] }, BLACK)));
	hexagonParts.push_back(unique_ptr<Quad>(new Quad({ vertices[1], vertices[2], vertices[3], vertices[4] }, BLACK)));*/

	hexagonParts.push_back(shared_ptr<Triangle>(new Triangle({ vertices[0], vertices[1], vertices[2] }, BLACK)));
	hexagonParts.push_back(shared_ptr<Triangle>(new Triangle({ vertices[3], vertices[4], vertices[5] }, BLACK)));
	hexagonParts.push_back(shared_ptr<Quad>(new Quad({ vertices[1], vertices[2], vertices[3], vertices[4] }, BLACK)));

	hexagonOverlay.push_back(shared_ptr<Triangle>(new Triangle({ vertices[0], vertices[1], vertices[2] }, ORANGE)));
	hexagonOverlay.push_back(shared_ptr<Triangle>(new Triangle({ vertices[3], vertices[4], vertices[5] }, ORANGE)));
	hexagonOverlay.push_back(shared_ptr<Quad>(new Quad({ vertices[1], vertices[2], vertices[3], vertices[4] }, ORANGE)));

	/*hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[0], vertices[1]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[1], vertices[3]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[3], vertices[5]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[5], vertices[4]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[4], vertices[2]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[2], vertices[0]))));*/

	hexagonStroke.push_back(shared_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[0], vertices[1]))));
	hexagonStroke.push_back(shared_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[1], vertices[3]))));
	hexagonStroke.push_back(shared_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[3], vertices[5]))));
	hexagonStroke.push_back(shared_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[5], vertices[4]))));
	hexagonStroke.push_back(shared_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[4], vertices[2]))));
	hexagonStroke.push_back(shared_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[2], vertices[0]))));

	for(int i = 0; i < SIDES; ++i)
	{
		int j = (i + 1) % SIDES;

		Vec2 strokeFixVertex1 = (*static_cast<Quad*>(hexagonStroke[i].get()))[2];
		Vec2 strokeFixVertex2 = (*static_cast<Quad*>(hexagonStroke[i].get()))[3];
		Vec2 strokeFixVertex3 = (*static_cast<Quad*>(hexagonStroke[j].get()))[1];

		//hexagonStroke.push_back(unique_ptr<Triangle>(new Triangle({ strokeFixVertex1, strokeFixVertex2, strokeFixVertex3 }, BLUE)));
		hexagonStroke.push_back(shared_ptr<Triangle>(new Triangle({ strokeFixVertex1, strokeFixVertex2, strokeFixVertex3 }, BLUE)));
	}
}

Quad Hexa::CalculateBorderQuad(const Vec2& v1, const Vec2& v2)
{
	if(v1.x == v2.x && v1.y == v2.y)
	{
		throw invalid_argument("Calculating the difference of the same two points.");
	}

	const double THICKNESS = 10.0;

	if(v2.y - v1.y == 0)
	{
		Vec2 direction = (v2.x > v1.x) ? Vec2(1, 0) : Vec2(-1, 0);

		Vec2 perpendicular = Vec2(-1 * direction.y, direction.x);
		perpendicular.x *= THICKNESS / 2;
		perpendicular.y *= THICKNESS / 2;

		return Quad({
			Vec2(v1.x + perpendicular.x, v1.y + perpendicular.y),
			Vec2(v1.x - perpendicular.x, v1.y - perpendicular.y),
			Vec2(v2.x + perpendicular.x, v2.y + perpendicular.y),
			Vec2(v2.x - perpendicular.x, v2.y - perpendicular.y)
		},
		SDL_Color(0, 255, 255, 255));
	}
	else if(v2.x - v1.x == 0)
	{
		Vec2 direction = (v2.y > v1.y) ? Vec2(1, 0) : Vec2(-1, 0);

		Vec2 perpendicular = Vec2(-1 * direction.y, direction.x);
		perpendicular.x *= THICKNESS / 2;
		perpendicular.y *= THICKNESS / 2;

		return Quad({
			Vec2(v1.x - perpendicular.x, v1.y - perpendicular.y),
			Vec2(v1.x + perpendicular.x, v1.y + perpendicular.y),
			Vec2(v2.x - perpendicular.x, v2.y - perpendicular.y),
			Vec2(v2.x + perpendicular.x, v2.y + perpendicular.y)
		}, 
		SDL_Color(0, 255, 255, 255));
	}
	else
	{
		Vec2 direction = Vec2(v2.x - v1.x, v2.y - v1.y);
		direction.Normalize();

		Vec2 perpendicular = Vec2(-1 * direction.y, direction.x);
		perpendicular.x *= THICKNESS / 2;
		perpendicular.y *= THICKNESS / 2;

		return Quad({
			Vec2(v1.x + perpendicular.x, v1.y + perpendicular.y),
			Vec2(v1.x - perpendicular.x, v1.y - perpendicular.y),
			Vec2(v2.x + perpendicular.x, v2.y + perpendicular.y),
			Vec2(v2.x - perpendicular.x, v2.y - perpendicular.y)
		}, 
		SDL_Color(0, 255, 255, 255));
	}

	throw invalid_argument("Subtracting two points should result in 0 change in x, 0 change in y, or some change in both x and y.");
}

void Hexa::Draw() const
{
	for(int i = 0; i < hexagonParts.size(); ++i)
	{
		(*hexagonParts[i].get()).Draw();

		if(overlayEnabled)
		{
			(*hexagonOverlay[i].get()).Draw();
		}
	}

	for(int i = 0; i < hexagonStroke.size(); ++i)
	{
		//(*hexagonStroke[i].get()).Draw();
	}
}

bool Hexa::Hit(SDL_Point& mousePos) const
{
	SDL_Rect rectangle = { static_cast<int>(nodes[1].x), static_cast<int>(nodes[1].y), Width(), Height() };

	// Check if mouse is in the middle rectangle
	if(SDL_PointInRect(&mousePos, &rectangle) == SDL_TRUE)
	{
		return true;
	}

	// Check if mouse is in the left triangle
	if((*static_cast<Triangle*>(hexagonParts[0].get())).Hit(mousePos.x, mousePos.y))
	{
		return true;
	}

	// Check if mouse is in the right triangle
	if((*static_cast<Triangle*>(hexagonParts[1].get())).Hit(mousePos.x, mousePos.y))
	{
		return true;
	}

	// Mouse is not inside the hexagon
	return false;
}

vector<Vec2> Hexa::_Vertices() const
{
	return nodes;
}

bool Hexa::operator==(const Hexa& hexa) const
{
	vector<Vec2> hexaPoints = hexa._Vertices();

	if (nodes[0] == hexaPoints[0] &&
		nodes[1] == hexaPoints[1] &&
		nodes[2] == hexaPoints[2] &&
		nodes[3] == hexaPoints[3] &&
		nodes[4] == hexaPoints[4] &&
		nodes[5] == hexaPoints[5])
	{
		return true;
	}

	return false;
}

bool Hexa::operator<(const Hexa& hexa) const
{
	return true;
}

Hexa Hexa::Translate(int x, int y) const
{
	return Hexa(
	{ 
		Vec2(nodes[0].x + x, nodes[0].y + y),
		Vec2(nodes[1].x + x, nodes[1].y + y),
		Vec2(nodes[2].x + x, nodes[2].y + y),
		Vec2(nodes[3].x + x, nodes[3].y + y),
		Vec2(nodes[4].x + x, nodes[4].y + y),
		Vec2(nodes[5].x + x, nodes[5].y + y),
	});
}

int Hexa::Width() const
{
	return nodes[4].x - nodes[1].x;
}

int Hexa::Height() const
{
	return nodes[4].y - nodes[1].y;
}

void Hexa::_Overlay(bool enabled)
{
	overlayEnabled = enabled;
}