// g++ main.cpp `pkg-config --cflags --libs sdl2`
#include <SDL.h>
#include <vector>
#include <format>
#include <initializer_list>

using namespace std;

struct vec2
{
    int x;
    int y;
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

    Quad(SDL_Renderer* renderer, const vector<vec2>& vertices) : renderer(renderer)
    {
        if(vertices.size() != 4)
        {
            throw invalid_argument(format("A quad should have 4 vertices. Not less, not more. The one you provided has {0:d}.", vertices.size()));
        }

        for(auto vertex : vertices)
        {
            this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertex.x, vertex.y), SDL_Color(255, 0, 0, 255), SDL_FPoint(0) });
        }

        this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertices[1].x, vertices[1].y), SDL_Color(255, 0, 0, 255), SDL_FPoint(0) });
        this->vertices.push_back((SDL_Vertex) { SDL_FPoint(vertices[2].x, vertices[2].y), SDL_Color(255, 0, 0, 255), SDL_FPoint(0) });
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
        quad = Quad(renderer, vector<vec2>({ vertices[1], vertices[2], vertices[3], vertices[4] }));
    }

    void Draw() const
    {
        triangles[0].Draw();
        triangles[1].Draw();
        quad.Draw();
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

        answer_a.Draw();
        answer_b.Draw();
        answer_c.Draw();
        answer_d.Draw();

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_RenderDrawLine(renderer, 0, 985, 1920, 985);

        SDL_RenderDrawLine(renderer, 0, 845, 1920, 845);

        SDL_RenderDrawLine(renderer, 1040, 0, 1040, 1080);

        SDL_RenderDrawLine(renderer, 1530, 0, 1530, 1080);

        SDL_RenderDrawLine(renderer, 390, 0, 390, 1080);

        SDL_RenderDrawLine(renderer, 880, 0, 880, 1080);

        SDL_RenderPresent( renderer );
    }

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}
