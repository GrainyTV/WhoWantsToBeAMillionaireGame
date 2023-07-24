#include "InGameScene.hpp"
#include "Scene.hpp"
#include "Game.hpp"

InGameScene::InGameScene() : question(make_tuple(Hexa({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) }), nullptr, false))
{
	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(340, 845), Vec2(390, 790), Vec2(390, 900), Vec2(896, 790), Vec2(896, 900), Vec2(946, 845) }),
			nullptr,
			false,
			[&, this] () { (*this).BeginGuessOnAnswer('A'); }
		)
	);

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(974, 845), Vec2(1024, 790), Vec2(1024, 900), Vec2(1530, 790), Vec2(1530, 900), Vec2(1580, 845) }),
			nullptr,
			false,
			[&, this] () { (*this).BeginGuessOnAnswer('B'); }
		)
	);

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(340, 985), Vec2(390, 930), Vec2(390, 1040), Vec2(896, 930), Vec2(896, 1040), Vec2(946, 985) }),
			nullptr,
			false,
			[&, this] () { (*this).BeginGuessOnAnswer('C'); }
		)
	);

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(974, 985), Vec2(1024, 930), Vec2(1024, 1040), Vec2(1530, 930), Vec2(1530, 1040), Vec2(1580, 985) }),
			nullptr,
			false,
			[&, this] () { (*this).BeginGuessOnAnswer('D'); }
		)
	);

	if(CheckTextboxPositionValidity() == false)
	{
		throw runtime_error("The 4 answer and 1 question textboxes do not line up.");
	}

	background = CreateTextureFromFile("background.png");

	buttonLines.push_back(Line(Vec2(0, 705), Vec2(Window::_Width(), 705), lineWidth, BLUE));
	buttonLines.push_back(Line(Vec2(0, 845), Vec2(Window::_Width(), 845), lineWidth, BLUE));
	buttonLines.push_back(Line(Vec2(0, 985), Vec2(Window::_Width(), 985), lineWidth, BLUE));
}

void InGameScene::Draw() const
{
	SDL_RenderCopy(Window::_Renderer(), background, NULL, NULL);

	SDL_Rect destination;

	for(auto line : buttonLines)
	{
		line.Draw();
	}

	get<Hexa>(question).Draw();
	destination = CenterTextInsideHexagon(get<Hexa>(question), get<SDL_Texture*>(question));

	if(get<bool>(question) == true)
	{
		SDL_RenderCopy(Window::_Renderer(), get<SDL_Texture*>(question), NULL, &destination);
	}
	
	for(auto button : buttons)
	{
		get<Hexa>(button).Draw();
		destination = CenterTextInsideHexagon(get<Hexa>(button), get<SDL_Texture*>(button));

		if(get<bool>(button) == true)
		{
			SDL_RenderCopy(Window::_Renderer(), get<SDL_Texture*>(button), NULL, &destination);
		}		
	}
}

bool InGameScene::CheckTextboxPositionValidity() const
{
	Hexa localQuestion = get<Hexa>(question);
	Hexa answer_a = get<Hexa>(buttons[0]);
	Hexa answer_b = get<Hexa>(buttons[1]);
	Hexa answer_c = get<Hexa>(buttons[2]);
	Hexa answer_d = get<Hexa>(buttons[3]);

	// A -> C
	Hexa translated_a1 = answer_a.Translate(0, 140);
	bool vertical = translated_a1 == answer_c;

	// A -> B
	Hexa translated_a2 = answer_a.Translate(634, 0);
	bool horizontal = translated_a2 == answer_b;

	// A -> D
	Hexa translated_a3 = answer_a.Translate(634, 140);
	bool diagonal = translated_a3 == answer_d;

	// Question -> A + B
	Hexa aCombinedb({ answer_a._Vertices()[0], answer_a._Vertices()[1], answer_a._Vertices()[2], answer_b._Vertices()[3], answer_b._Vertices()[4], answer_b._Vertices()[5] });
	bool verticalScaled = localQuestion == aCombinedb.Translate(0, -140);

	return  (vertical && horizontal && diagonal && verticalScaled) ? true : false;
}

void InGameScene::InitiateNewGame()
{
	currentGame = make_unique<NewGame>((*Game::Instance()).GenerateNewGame());

	(*this).LoadTextures();
	thread createNewRound { [&, this] () { (*this).NewRound(); } };
	createNewRound.detach();
}

void InGameScene::BeginGuessOnAnswer(char guessedLetter)
{
	guessedLetterIdx = guessedLetter - 'A';

	if((*currentGame.get()).GuessAnswer(guessedLetter) == true)
	{
		thread answerValidationTask(&InGameScene::AnswerValidationAnimation, this, true);
		answerValidationTask.detach();

		(*this).LoadTextures();

		thread createNewRound(&InGameScene::NewRound, this);
		createNewRound.detach();
	}
	else
	{
		gameState = GameState::Lost;
	}
}

void InGameScene::NewRound()
{
	(*this)._MouseEnabled(false);

	unique_lock lock(animationMutex);
	lockHandler.wait(lock, [&, this] { return answerValidationAnimationFinished; });
	answerValidationAnimationFinished = false;

	if((*currentGame.get())._Round() % 5 == 0)
	{
		sleep_for(5000ms);
	}
	else
	{
		sleep_for(2000ms);
	}
	
	get<SDL_Texture*>(question) = textureHolder.front();
	textureHolder.pop_front();
	get<bool>(question) = true;
	Scene::Instance().Invalidate();

	for(auto& button : buttons)
	{
		sleep_for(2000ms);
		get<SDL_Texture*>(button) = textureHolder.front();
		textureHolder.pop_front();
		get<bool>(button) = true;
		Scene::Instance().Invalidate();
	}

	(*this)._MouseEnabled(true);
}

void InGameScene::AnswerValidationAnimation(bool success)
{
	// Validation of answer (Orange color)
	(*this)._MouseEnabled(false);
	sleep_for(2000ms);

	if(success)
	{
		// Correct answer (Green color)
		get<Hexa>(buttons[guessedLetterIdx]).ChangeColor(SDL_Color{ 14, 146, 19, 255 });
		Scene::Instance().Invalidate();
		sleep_for(2000ms);

		// Wait for new question (Black color)
		get<Hexa>(buttons[guessedLetterIdx]).ChangeColor(SDL_Color{ 0, 0, 0, 255 });
		Scene::Instance().Invalidate();
	}
	else
	{

	}

	get<bool>(question) = false;
	get<bool>(buttons[0]) = false;
	get<bool>(buttons[1]) = false;
	get<bool>(buttons[2]) = false;
	get<bool>(buttons[3]) = false;

	SDL_DestroyTexture(get<SDL_Texture*>(question));
	if(string(SDL_GetError()).compare("Parameter 'texture' is invalid") == 0)
	{
		throw runtime_error("Failed to destroy question texture in AnswerValidationAnimation function!");
	}

	for(auto& button : buttons)
	{
		SDL_DestroyTexture(get<SDL_Texture*>(button));
		if(string(SDL_GetError()).compare("Parameter 'texture' is invalid") == 0)
		{
			throw runtime_error("Failed to destroy button texture in AnswerValidationAnimation function!");
		}
	}

	{
		lock_guard lock(animationMutex);
		answerValidationAnimationFinished = true;
	}

	lockHandler.notify_one();
}

void InGameScene::LoadTextures()
{
	textureHolder.push_back(CreateTextureFromText((*currentGame.get()).ThisRoundsData()._Question(), "question"));
	textureHolder.push_back(CreateTextureFromText((*currentGame.get()).ThisRoundsData()._A(), "answer"));
	textureHolder.push_back(CreateTextureFromText((*currentGame.get()).ThisRoundsData()._B(), "answer"));
	textureHolder.push_back(CreateTextureFromText((*currentGame.get()).ThisRoundsData()._C(), "answer"));
	textureHolder.push_back(CreateTextureFromText((*currentGame.get()).ThisRoundsData()._D(), "answer"));
}