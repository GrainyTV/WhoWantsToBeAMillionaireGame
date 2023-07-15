#include "MainMenuScene.hpp"
#include "Window.hpp"

MainMenuScene::MainMenuScene()
:
startGame({ Vec2(657, 704), Vec2(707, 649), Vec2(707, 759), Vec2(1213, 649), Vec2(1213, 759), Vec2(1263, 704) }),
options({ Vec2(657, 844), Vec2(707, 789), Vec2(707, 899), Vec2(1213, 789), Vec2(1213, 899), Vec2(1263, 844) }),
quit({ Vec2(657, 984), Vec2(707, 929), Vec2(707, 1039), Vec2(1213, 929), Vec2(1213, 1039), Vec2(1263, 984) })
{
	if(CheckTextboxPositionValidity() == false)
	{
		throw runtime_error("The 3 buttons in the main menu don't line up.");
	}

	startGameText = CreateTextureFromText("Új játék", "answer");
	optionsText = CreateTextureFromText("Opciók", "answer");
	quitText = CreateTextureFromText("Kilépés", "answer");
}

void MainMenuScene::Draw() const
{
	SDL_RenderCopy(Window::_Renderer(), background, NULL, NULL);

	SDL_Rect destination;

	//destination = SDL_Rect {  };
	//SDL_RenderCopy(Window::_Renderer(), logo, NULL, &destination);

	startGame.Draw();
	options.Draw();
	quit.Draw();

	destination = CenterTextInsideHexagon(startGame, startGameText);
	SDL_RenderCopy(Window::_Renderer(), startGameText, NULL, &destination);

	destination = CenterTextInsideHexagon(options, optionsText);
	SDL_RenderCopy(Window::_Renderer(), optionsText, NULL, &destination);

	destination = CenterTextInsideHexagon(quit, quitText);
	SDL_RenderCopy(Window::_Renderer(), quitText, NULL, &destination);
}

bool MainMenuScene::CheckTextboxPositionValidity() const
{
	// StartGame -> Options
	Hexa translatedStartGame = startGame.Translate(0, 140);
	bool verticalFirst = translatedStartGame == options;

	// StartGame -> Quit
	Hexa translatedOptions = options.Translate(0, 140);
	bool verticalSecond = translatedOptions == quit;

	return (verticalFirst && verticalSecond) ? true : false;
}