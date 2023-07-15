#include "SDL_ttf.h"
#include "DrawableScene.hpp"
#include "Window.hpp"

SDL_Surface* DrawableScene::CreateSurfaceFromText(const string& text, const string& type) const
{
	// Color used is white
	const SDL_Color WHITE = { 255, 255, 255 };

	// Available width (Does not matter which box we use, all have same size)
	const unsigned int WIDTH = (type.compare("answer") == 0) ? 506 : 1140;

	// Available height
	const unsigned int HEIGHT = 110;

	// Default Font Size used is 32 pixels
	unsigned int size = 32;

	// Our returned text surface
	SDL_Surface* textSurface;

	// Loop termination condition
	bool foundMatchingSize;

	// The font we use
	TTF_Font* arialFont;

	do
	{
		foundMatchingSize = true;
		arialFont = TTF_OpenFont("font/FontsFree-Net-arial-bold.ttf", size);

		if(arialFont == NULL)
		{
			throw runtime_error("Could not open font.");
		}

		textSurface = TTF_RenderUTF8_Blended_Wrapped(arialFont, text.c_str(), WHITE, WIDTH);

		if(textSurface == NULL)
		{
			throw runtime_error("Could not create surface from text.");
		}

		if((*textSurface).w > WIDTH || (*textSurface).h > HEIGHT)
		{
			foundMatchingSize = false;
			SDL_FreeSurface(textSurface);
			TTF_CloseFont(arialFont);

			// Try 1 pixel smaller font
			size -= 1;

			if(size == 0)
			{
				throw runtime_error("Font size was reduced to 0.");
			}
		}

	} while(foundMatchingSize == false);

	TTF_CloseFont(arialFont);	
	return textSurface;
}

SDL_Texture* DrawableScene::CreateTextureFromText(const string& text, const string& type) const
{	
	future<SDL_Surface*> loadingTask = async(launch::async, &DrawableScene::CreateSurfaceFromText, this, text, type);
	SDL_Surface* loadedSurface = loadingTask.get();

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::_Renderer(), loadedSurface);

	if(texture == NULL)
	{
		throw runtime_error("Could not convert surface to texture.");
	}

	SDL_FreeSurface(loadedSurface);
	return texture;
}

SDL_Rect DrawableScene::CenterTextInsideHexagon(const Hexa& hexagon, SDL_Texture* text) const
{
	SDL_Point querySize;
	SDL_QueryTexture(text, NULL, NULL, &querySize.x, &querySize.y);
	const int WIDTH = querySize.x;
	const int HEIGHT = querySize.y;
	
	SDL_Rect rectangle = 
	{ 
		static_cast<int>(hexagon._Vertices()[1].x) + (hexagon.Width() - WIDTH) / 2,
		static_cast<int>(hexagon._Vertices()[1].y) + (hexagon.Height() - HEIGHT) / 2,
		WIDTH,
		HEIGHT
	};
	
	return rectangle;
}