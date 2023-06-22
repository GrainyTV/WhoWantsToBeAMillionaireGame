#include "Hexa.hpp"

Hexa::Hexa(const initializer_list<Vec2>& vertexList) : Primitive()
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
	
	hexagonParts.push_back(unique_ptr<Triangle>(new Triangle({ vertices[0], vertices[1], vertices[2] }, SDL_Color(255, 0, 0, 255))));
	hexagonParts.push_back(unique_ptr<Triangle>(new Triangle({ vertices[3], vertices[4], vertices[5] }, SDL_Color(255, 0, 0, 255))));
	hexagonParts.push_back(unique_ptr<Quad>(new Quad({ vertices[1], vertices[2], vertices[3], vertices[4] }, SDL_Color(255, 0, 0, 255))));

	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[0], vertices[1]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[1], vertices[3]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[3], vertices[5]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[5], vertices[4]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[4], vertices[2]))));
	hexagonStroke.push_back(unique_ptr<Quad>(new Quad(CalculateBorderQuad(vertices[2], vertices[0]))));

	for(int i = 0; i < SIDES; ++i)
	{
		int j = (i + 1) % SIDES;

		Vec2 strokeFixVertex1 = (*static_cast<Quad*>(hexagonStroke[i].get()))[2];
		Vec2 strokeFixVertex2 = (*static_cast<Quad*>(hexagonStroke[i].get()))[3];
		Vec2 strokeFixVertex3 = (*static_cast<Quad*>(hexagonStroke[j].get()))[1];

		hexagonStroke.push_back(unique_ptr<Triangle>(new Triangle({ strokeFixVertex1, strokeFixVertex2, strokeFixVertex3 }, SDL_Color(0, 255, 255, 255))));
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
	}

	for(int i = 0; i < hexagonStroke.size(); ++i)
	{
		(*hexagonStroke[i].get()).Draw();
	}
}

vector<Vec2> Hexa::_Vertices() const
{
	return nodes;
}

double Hexa::CalculateHexagonArea(const vector<Vec2>& hexagonCoordinates)
{
	double area = 0.0;
	
	double height = hexagonCoordinates[2].y - hexagonCoordinates[1].y;
	double width = hexagonCoordinates[3].x - hexagonCoordinates[1].x;
	double rectangleArea = width * height;
	
	double triangleArea1 = abs(
		(
		hexagonCoordinates[0].x * (hexagonCoordinates[1].y - hexagonCoordinates[2].y)
		+
		hexagonCoordinates[1].x * (hexagonCoordinates[2].y - hexagonCoordinates[0].y)
		+
		hexagonCoordinates[2].x * (hexagonCoordinates[0].y - hexagonCoordinates[1].y)
		)
	/
	2.0
	);
	
	double triangleArea2 = abs(
		(
		hexagonCoordinates[3].x * (hexagonCoordinates[4].y - hexagonCoordinates[5].y)
		+
		hexagonCoordinates[4].x * (hexagonCoordinates[5].y - hexagonCoordinates[3].y)
		+
		hexagonCoordinates[5].x * (hexagonCoordinates[3].y - hexagonCoordinates[4].y)
		)
	/
	2.0
	);

	return rectangleArea + triangleArea1 + triangleArea2;
}