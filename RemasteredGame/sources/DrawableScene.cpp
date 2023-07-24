#include "SDL_ttf.h"
#include "SDL_image.h"
#include "DrawableScene.hpp"
#include "Window.hpp"

/**
 * 
 * Getter function for the mouseEnabled property.
 * 
 */
bool DrawableScene::_MouseEnabled() const
{
	return mouseEnabled;
}

/**
 * 
 * Setter function for the mouseEnabled property.
 * 
 */
void DrawableScene::_MouseEnabled(bool enabled)
{
	mouseEnabled = enabled;
}

/**
 * 
 * Intersection test for the currently applied scene's buttons.
 * @param mousePos : the location of the cursor when the method was called
 * @return : the index of the button hit, 0 if it is not a clickable surface
 * 
 */
unsigned int DrawableScene::Hit(SDL_Point mousePos)
{
	int idx = -1;

	for(int i = 0; i < buttons.size(); ++i)
	{
		if(get<Hexa>(buttons[i]).Hit(mousePos))
		{
			idx = i;
			//get<Hexa>(buttons[i])._Overlay(true);
			get<Hexa>(buttons[i]).ChangeColor(SDL_Color{ 254, 125, 5, 255 });
		}
		else
		{
			//get<Hexa>(buttons[i])._Overlay(false);
			get<Hexa>(buttons[i]).ChangeColor(SDL_Color{ 0, 0, 0, 255 });
		}
	}

	return idx + 1;
}

/**
 * 
 * Delegate call based on what button the user had clicked.
 * @param index : the index of the clicked button
 * 
 */
void DrawableScene::Invoke(int index) const
{
	get<function<void()>>(buttons[index])();
}

/**
 * 
 * Sub-function for loading a texture from disk. Used in async mode.
 * @param file : the name and extension of the file
 * @return : either a loaded surface or an exception
 * 
 */
SDL_Surface* DrawableScene::CreateSurfaceFromFile(const string& file) const
{
	path fileWithDirectory = "textures";
	fileWithDirectory /= file;

	SDL_Surface* loaded = IMG_Load(fileWithDirectory.c_str());

	if(loaded == NULL)
	{
		throw runtime_error(format("Could not create surface from file: {0:s}", fileWithDirectory.string()));
	}

	return loaded;
}

/**
 * 
 * Function for loading a texture from disk.
 * @param file : the name and extension of the file
 * @return : either a loaded texture or an exception
 * 
 */
SDL_Texture* DrawableScene::CreateTextureFromFile(const string& file) const
{
	future<SDL_Surface*> loadingTask = async(launch::async, &DrawableScene::CreateSurfaceFromFile, this, file);
	SDL_Surface* loadedSurface = loadingTask.get();

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::_Renderer(), loadedSurface);

	if(texture == NULL)
	{
		throw runtime_error(format("Could not convert surface {0:s} to texture.", file));
	}

	SDL_FreeSurface(loadedSurface);

	return texture;
}

/**
 * 
 * Sub-function for creating a texture from string. Used in async mode.
 * @param text : the text we convert to texture
 * @param type : wether the text is used as answer or question (the latter needs more space)
 * @return : either a loaded surface or an exception
 * 
 */
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

/**
 * 
 * Function for creating a texture from string.
 * @param text : the text we convert to texture
 * @param type : wether the text is used as answer or question (the latter needs more space)
 * @return : either a loaded texture or an exception
 * 
 */
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

/**
 * 
 * Helper function to center text inside a hexagon horizontally and vertically.
 * @param hexagon : which hexagon to use
 * @param text : what text to include
 * @return : the calculated rectangular area
 * 
 */
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