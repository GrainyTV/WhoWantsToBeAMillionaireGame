#include <SDL.h>
#include <vector>
#include <format>
#include <initializer_list>
#include <cmath>
#include <stdexcept>
#include "Window.hpp"
#include "Hexa.hpp"

using namespace std;

int main( int argc, char** argv )
{
	SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);

	Window w1;

	SDL_Window* window = w1._Window();
	SDL_Renderer* renderer = w1._Renderer();
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	const Hexa answer_a({ Vec2(340, 845), Vec2(390, 790), Vec2(390, 900), Vec2(896, 790), Vec2(896, 900), Vec2(946, 845) });
	const Hexa answer_b({ Vec2(974, 845), Vec2(1024, 790), Vec2(1024, 900), Vec2(1530, 790), Vec2(1530, 900), Vec2(1580, 845) });
	const Hexa answer_c({ Vec2(340, 985), Vec2(390, 930), Vec2(390, 1040), Vec2(896, 930), Vec2(896, 1040), Vec2(946, 985) });
	const Hexa answer_d({ Vec2(974, 985), Vec2(1024, 930), Vec2(1024, 1040), Vec2(1530, 930), Vec2(1530, 1040), Vec2(1580, 985) });
	const Hexa question({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) });

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

		SDL_RenderPresent( renderer );
	}

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
