#include "MainMenuScene.hpp"
#include "Window.hpp"

/**
 * 
 * Constructor for setting initial values for the main menu screen.
 * 
 */
MainMenuScene::MainMenuScene() : lineWidth(5), BLUE(SDL_Color(95, 194, 253, 255))
{
	buttons.push_back(
		make_pair(Hexa({ Vec2(657, 704), Vec2(707, 649), Vec2(707, 759), Vec2(1213, 649), Vec2(1213, 759), Vec2(1263, 704) }), 
		CreateTextureFromText("Új játék", "answer"))
	);
	
	buttons.push_back(
		make_pair(Hexa({ Vec2(657, 844), Vec2(707, 789), Vec2(707, 899), Vec2(1213, 789), Vec2(1213, 899), Vec2(1263, 844) }),
		CreateTextureFromText("Opciók", "answer"))
	);
	
	buttons.push_back(
		make_pair(Hexa({ Vec2(657, 984), Vec2(707, 929), Vec2(707, 1039), Vec2(1213, 929), Vec2(1213, 1039), Vec2(1263, 984) }),
		CreateTextureFromText("Kilépés", "answer"))
	);

	if(CheckTextboxPositionValidity() == false)
	{
		throw runtime_error("The 3 buttons in the main menu don't line up.");
	}

	background = CreateTextureFromFile("background.png");
	logo = CreateTextureFromFile("logo.png");
	
	buttonLines.push_back(Line(Vec2(0, 704), Vec2(Window::_Width(), 704), lineWidth, BLUE));
	buttonLines.push_back(Line(Vec2(0, 844), Vec2(Window::_Width(), 844), lineWidth, BLUE));
	buttonLines.push_back(Line(Vec2(0, 984), Vec2(Window::_Width(), 984), lineWidth, BLUE));
}

/**
 * 
 * Render function for the main menu.
 * 
 */
void MainMenuScene::Draw() const
{
	SDL_RenderCopy(Window::_Renderer(), background, NULL, NULL);

	SDL_Rect destination;

	destination = SDL_Rect { 665, 30, 590, 590 };
	SDL_RenderCopy(Window::_Renderer(), logo, NULL, &destination);

	for(auto line : buttonLines)
	{
		line.Draw();
	}

	for(auto button : buttons)
	{
		button.first.Draw();
		destination = CenterTextInsideHexagon(button.first, button.second);
		SDL_RenderCopy(Window::_Renderer(), button.second, NULL, &destination);
	}
}


/**
 * 
 * Intersection test for the MainMenu scene's buttons.
 * @param mousePos : the location of the cursor when the method was called
 * @return : the index of the button hit, 0 if it is not a clickable surface
 * 
 */
unsigned int MainMenuScene::Hit(SDL_Point mousePos)
{
	int idx = -1;

	for(int i = 0; i < buttons.size(); ++i)
	{
		if(buttons[i].first.Hit(mousePos))
		{
			idx = i;
			buttons[i].first._Overlay(true);
		}
		else
		{
			buttons[i].first._Overlay(false);
		}
	}

	return idx + 1;
}

/**
 * 
 * Helper function to check wether our clickable buttons are all same sizes and at their desired locations.
 * 
 */
bool MainMenuScene::CheckTextboxPositionValidity() const
{
	Hexa startGame = buttons[0].first;
	Hexa options = buttons[1].first;
	Hexa quit = buttons[2].first;
	
	// StartGame -> Options
	Hexa translatedStartGame = startGame.Translate(0, 140);
	bool verticalFirst = translatedStartGame == options;

	// StartGame -> Quit	
	Hexa translatedOptions = options.Translate(0, 140);
	bool verticalSecond = translatedOptions == quit;

	return (verticalFirst && verticalSecond) ? true : false;
}