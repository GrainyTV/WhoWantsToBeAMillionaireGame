#include "MainMenuScene.hpp"
#include "Window.hpp"

/**
 * 
 * Constructor for setting initial values for the main menu screen.
 * 
 */
MainMenuScene::MainMenuScene()
//:
//startGame({ Vec2(657, 704), Vec2(707, 649), Vec2(707, 759), Vec2(1213, 649), Vec2(1213, 759), Vec2(1263, 704) }),
//options({ Vec2(657, 844), Vec2(707, 789), Vec2(707, 899), Vec2(1213, 789), Vec2(1213, 899), Vec2(1263, 844) }),
//quit({ Vec2(657, 984), Vec2(707, 929), Vec2(707, 1039), Vec2(1213, 929), Vec2(1213, 1039), Vec2(1263, 984) })
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
	
	//startGameText = CreateTextureFromText("Új játék", "answer");
	//optionsText = CreateTextureFromText("Opciók", "answer");
	//quitText = CreateTextureFromText("Kilépés", "answer");
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

	for(auto button : buttons)
	{
		button.first.Draw();
		destination = CenterTextInsideHexagon(button.first, button.second);
		SDL_RenderCopy(Window::_Renderer(), button.second, NULL, &destination);
	}
	
	//startGame.Draw();
	//options.Draw();
	//quit.Draw();

	/*destination = CenterTextInsideHexagon(startGame, startGameText);
	SDL_RenderCopy(Window::_Renderer(), startGameText, NULL, &destination);

	destination = CenterTextInsideHexagon(options, optionsText);
	SDL_RenderCopy(Window::_Renderer(), optionsText, NULL, &destination);

	destination = CenterTextInsideHexagon(quit, quitText);
	SDL_RenderCopy(Window::_Renderer(), quitText, NULL, &destination);*/
}

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