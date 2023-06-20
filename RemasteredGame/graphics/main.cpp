// g++ main.cpp `pkg-config --cflags --libs sdl2`
#include <SDL.h>
#include <vector>
#include <format>
#include <initializer_list>
#include <cmath>
#include <stdexcept>

using namespace std;

struct vec2
{
	double x;
	double y;

	void normalize()
	{
		double distance = sqrt(pow(x, 2) + pow(y, 2));
		x /= distance;
		y /= distance;
	}
};

class Quad
{
private:
	SDL_Renderer* renderer;

	vector<SDL_Vertex> vertices;

public:
	Quad()
	{
	}

	Quad(SDL_Renderer* renderer, const vector<vec2>& vertices, const SDL_Color& color) : renderer(renderer)
	{
		if(vertices.size() != 4)
		{
			throw invalid_argument(format("A quad should have 4 vertices. Not less, not more. The one you provided has {0:d}.", vertices.size()));
		}

		for(auto vertex : vertices)
		{
			this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertex.x, vertex.y), SDL_Color(color.r, color.g, color.g, color.a), SDL_FPoint(0) });
		}

		this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertices[1].x, vertices[1].y), SDL_Color(color.r, color.g, color.g, color.a), SDL_FPoint(0) });
		this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertices[2].x, vertices[2].y), SDL_Color(color.r, color.g, color.g, color.a), SDL_FPoint(0) });
	}

	void Draw() const 
	{
		SDL_RenderGeometry( renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
	}
};

class Triangle
{
private:
	SDL_Renderer* renderer;

	vector<SDL_Vertex> vertices;

public:
	Triangle()
	{
	}

	Triangle(SDL_Renderer* renderer, const vector<vec2>& vertices) : renderer(renderer)
	{
		if(vertices.size() != 3)
		{
			throw invalid_argument(format("A triangle should have 3 vertices. Not less, not more. The one you provided has {0:d}.", vertices.size()));
		}

		for(auto vertex : vertices)
		{
			this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertex.x, vertex.y), SDL_Color(255, 0, 0, 255), SDL_FPoint(0) });
		}
	}

	void Draw() const 
	{
		SDL_RenderGeometry( renderer, NULL, vertices.data(), vertices.size(), NULL, 0);
	}
};

class Hexa
{
private:
	vector<Quad> stroke;

	SDL_Renderer* renderer;
	
	Triangle triangles[2];
	
	Quad quad;

public:
	Hexa(SDL_Renderer* renderer, const std::vector<vec2>& vertices) : renderer(renderer)
	{
		if(vertices.size() != 6)
		{
			throw invalid_argument(format("A hexagon should have 6 vertices. Not less, not more. The one you provided has {0:d}.", vertices.size()));
		}

		triangles[0] = Triangle(renderer, vector<vec2>({ vertices[0], vertices[1], vertices[2] }));
		triangles[1] = Triangle(renderer, vector<vec2>({ vertices[3], vertices[4], vertices[5] }));
		quad = Quad(renderer, vector<vec2>({ vertices[1], vertices[2], vertices[3], vertices[4] }), SDL_Color(255, 0, 0, 255));

		stroke = vector<Quad>({
			CalulateBorderQuad(renderer, vertices[0], vertices[1]),
			CalulateBorderQuad(renderer, vertices[1], vertices[3]),
			CalulateBorderQuad(renderer, vertices[3], vertices[5]),
			CalulateBorderQuad(renderer, vertices[5], vertices[4]),
			CalulateBorderQuad(renderer, vertices[4], vertices[2]),
			CalulateBorderQuad(renderer, vertices[2], vertices[0])
		});
	}

	Quad CalulateBorderQuad(SDL_Renderer* renderer, const vec2& v1, const vec2& v2)
	{
		if(v1.x == v2.x && v1.y == v2.y)
		{
			throw invalid_argument("Calculating the difference of the same two points.");
		}

		const double THICKNESS = 10.0;

		if(v2.y - v1.y == 0)
		{
			vec2 direction = (v2.y > v1.y) ? vec2(1, 0) : vec2(-1, 0);

			vec2 perpendicular = vec2(-1 * direction.y, direction.x);
			perpendicular.x *= THICKNESS / 2;
			perpendicular.y *= THICKNESS / 2;

			return Quad(renderer, vector<vec2>({
				vec2(v1.x - perpendicular.x, v1.y - perpendicular.y),
				vec2(v1.x + perpendicular.x, v1.y + perpendicular.y),
				vec2(v2.x - perpendicular.x, v2.y - perpendicular.y),
				vec2(v2.x + perpendicular.x, v2.y + perpendicular.y)
			}),
				SDL_Color(0, 255, 0, 255)
			);
		}
		else if(v2.x - v1.x == 0)
		{
			vec2 direction = (v2.x > v1.x) ? vec2(1, 0) : vec2(-1, 0);

			vec2 perpendicular = vec2(-1 * direction.y, direction.x);
			perpendicular.x *= THICKNESS / 2;
			perpendicular.y *= THICKNESS / 2;

			return Quad(renderer, vector<vec2>({
				vec2(v1.x - perpendicular.x, v1.y - perpendicular.y),
				vec2(v1.x + perpendicular.x, v1.y + perpendicular.y),
				vec2(v2.x - perpendicular.x, v2.y - perpendicular.y),
				vec2(v2.x + perpendicular.x, v2.y + perpendicular.y)
			}),
				SDL_Color(0, 255, 0, 255)
			);
		}
		else
		{
			vec2 direction = vec2(v2.x - v1.x, v2.y - v1.y);
			direction.normalize();

			vec2 perpendicular = vec2(-1 * direction.y, direction.x);
			perpendicular.x *= THICKNESS / 2;
			perpendicular.y *= THICKNESS / 2;

			return Quad(renderer, vector<vec2>({
				vec2(v1.x - perpendicular.x, v1.y - perpendicular.y),
				vec2(v1.x + perpendicular.x, v1.y + perpendicular.y),
				vec2(v2.x - perpendicular.x, v2.y - perpendicular.y),
				vec2(v2.x + perpendicular.x, v2.y + perpendicular.y)
			}),
				SDL_Color(0, 255, 0, 255)
			);
		}

		throw invalid_argument("Subtracting two points should result in 0 change in x, 0 change in y, or some change in both x and y.");
	}

	void Draw() const
	{
		triangles[0].Draw();
		triangles[1].Draw();
		quad.Draw();
		
		for(auto quad : stroke)
		{
			quad.Draw();
		}
	}
};

int main( int argc, char** argv )
{
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	SDL_Window* window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	SDL_Renderer* renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	const Hexa answer_a(renderer, vector<vec2>({ vec2(340, 845), vec2(390, 790), vec2(390, 900), vec2(880, 790), vec2(880, 900), vec2(930, 845) }));
	const Hexa answer_b(renderer, vector<vec2>({ vec2(990, 845), vec2(1040, 790), vec2(1040, 900), vec2(1530, 790), vec2(1530, 900), vec2(1580, 845) }));
	const Hexa answer_c(renderer, vector<vec2>({ vec2(340, 985), vec2(390, 930), vec2(390, 1040), vec2(880, 930), vec2(880, 1040), vec2(930, 985) }));
	const Hexa answer_d(renderer, vector<vec2>({ vec2(990, 985), vec2(1040, 930), vec2(1040, 1040), vec2(1530, 930), vec2(1530, 1040), vec2(1580, 985) }));
	const Hexa question(renderer, vector<vec2>({ vec2(340, 705), vec2(390, 650), vec2(390, 760), vec2(1530, 650), vec2(1530, 760), vec2(1580, 705) }));

	bool running = true;
	while( running )
	{
		SDL_Event ev;
		while( SDL_PollEvent( &ev ) )
		{
			if( ( SDL_QUIT == ev.type ) ||
				( SDL_KEYDOWN == ev.type && SDL_SCANCODE_ESCAPE == ev.key.keysym.scancode ) )
			{
				running = false;
				break;
			}
		}

		SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
		SDL_RenderClear( renderer ); 

		question.Draw();
		answer_a.Draw();
		answer_b.Draw();
		answer_c.Draw();
		answer_d.Draw();

		/*SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

		SDL_RenderDrawLine(renderer, 0, 985, 1920, 985);

		SDL_RenderDrawLine(renderer, 0, 845, 1920, 845);

		SDL_RenderDrawLine(renderer, 1040, 0, 1040, 1080);

		SDL_RenderDrawLine(renderer, 1530, 0, 1530, 1080);

		SDL_RenderDrawLine(renderer, 390, 0, 390, 1080);

		SDL_RenderDrawLine(renderer, 880, 0, 880, 1080);*/

		SDL_RenderPresent( renderer );
	}

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
