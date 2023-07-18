#include "MainMenuScene.hpp"
#include "Game.hpp"
#include "Scene.hpp"

/**
 * 
 * Constructor for setting initial values for the main menu screen.
 * 
 */
MainMenuScene::MainMenuScene()
{
	//
	// Button for starting a new game
	//
	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(657, 704), Vec2(707, 649), Vec2(707, 759), Vec2(1213, 649), Vec2(1213, 759), Vec2(1263, 704) }), 
			CreateTextureFromText("Új játék", "answer"),
			[&, this] () { Scene::Instance().ChangeScene("InGame"); }
		)
	);
	
	//
	// Button for options
	//
	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(657, 844), Vec2(707, 789), Vec2(707, 899), Vec2(1213, 789), Vec2(1213, 899), Vec2(1263, 844) }),
			CreateTextureFromText("Opciók", "answer"),
			[&, this] () { /* TODO */ }
		)
	);
	
	//
	// Button for quitting
	//
	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(657, 984), Vec2(707, 929), Vec2(707, 1039), Vec2(1213, 929), Vec2(1213, 1039), Vec2(1263, 984) }),
			CreateTextureFromText("Kilépés", "answer"),
			[&, this] () { Game::Terminate(); }
		)
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
		get<Hexa>(button).Draw();
		destination = CenterTextInsideHexagon(get<Hexa>(button), get<SDL_Texture*>(button));
		SDL_RenderCopy(Window::_Renderer(), get<SDL_Texture*>(button), NULL, &destination);
	}
}

/**
 * 
 * Helper function to check wether our clickable buttons are all same sizes and at their desired locations.
 * 
 */
bool MainMenuScene::CheckTextboxPositionValidity() const
{
	Hexa startGame = get<Hexa>(buttons[0]);
	Hexa options = get<Hexa>(buttons[1]);
	Hexa quit = get<Hexa>(buttons[2]);
	
	// StartGame -> Options
	Hexa translatedStartGame = startGame.Translate(0, 140);
	bool verticalFirst = translatedStartGame == options;

	// StartGame -> Quit	
	Hexa translatedOptions = options.Translate(0, 140);
	bool verticalSecond = translatedOptions == quit;

	return (verticalFirst && verticalSecond) ? true : false;
}