#include "InGameScene.hpp"
#include "Window.hpp"
#include "Game.hpp"

InGameScene::InGameScene()
:
question(
	make_tuple(
		Hexa({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) }),
		nullptr
	)
),
lineWidth(5),
BLUE(SDL_Color(95, 194, 253, 255))
/*:
question({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) }),
answer_a({ Vec2(340, 845), Vec2(390, 790), Vec2(390, 900), Vec2(896, 790), Vec2(896, 900), Vec2(946, 845) }),
answer_b({ Vec2(974, 845), Vec2(1024, 790), Vec2(1024, 900), Vec2(1530, 790), Vec2(1530, 900), Vec2(1580, 845) }),
answer_c({ Vec2(340, 985), Vec2(390, 930), Vec2(390, 1040), Vec2(896, 930), Vec2(896, 1040), Vec2(946, 985) }),
answer_d({ Vec2(974, 985), Vec2(1024, 930), Vec2(1024, 1040), Vec2(1530, 930), Vec2(1530, 1040), Vec2(1580, 985) })*/
{
	/*question = make_tuple(
		Hexa({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) }),
		static_cast<SDL_Texture*>(nullptr)
	);*/

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(340, 845), Vec2(390, 790), Vec2(390, 900), Vec2(896, 790), Vec2(896, 900), Vec2(946, 845) }),
			nullptr,
			[&, this] () { (*currentGame.get()).GuessAnswer('A'); }
		)
	);

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(974, 845), Vec2(1024, 790), Vec2(1024, 900), Vec2(1530, 790), Vec2(1530, 900), Vec2(1580, 845) }),
			nullptr,
			[&, this] () { (*currentGame.get()).GuessAnswer('B'); }
		)
	);

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(340, 985), Vec2(390, 930), Vec2(390, 1040), Vec2(896, 930), Vec2(896, 1040), Vec2(946, 985) }),
			nullptr,
			[&, this] () { (*currentGame.get()).GuessAnswer('C'); }
		)
	);

	buttons.push_back(
		make_tuple(
			Hexa({ Vec2(974, 985), Vec2(1024, 930), Vec2(1024, 1040), Vec2(1530, 930), Vec2(1530, 1040), Vec2(1580, 985) }),
			nullptr,
			[&, this] () { (*currentGame.get()).GuessAnswer('D'); }
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
	SDL_RenderCopy(Window::_Renderer(), get<SDL_Texture*>(question), NULL, &destination);
	
	for(auto button : buttons)
	{
		get<Hexa>(button).Draw();
		destination = CenterTextInsideHexagon(get<Hexa>(button), get<SDL_Texture*>(button));
		SDL_RenderCopy(Window::_Renderer(), get<SDL_Texture*>(button), NULL, &destination);
	}

	/*question.Draw();
	answer_a.Draw();
	answer_b.Draw();
	answer_c.Draw();
	answer_d.Draw();

	SDL_Rect destination;

	destination = CenterTextInsideHexagon(question, questionText);
	SDL_RenderCopy(Window::_Renderer(), questionText, NULL, &destination);
	
	destination = CenterTextInsideHexagon(answer_a, answer_aText);
	SDL_RenderCopy(Window::_Renderer(), answer_aText, NULL, &destination);

	destination = CenterTextInsideHexagon(answer_b, answer_bText);
	SDL_RenderCopy(Window::_Renderer(), answer_bText, NULL, &destination);

	destination = CenterTextInsideHexagon(answer_c, answer_cText);
	SDL_RenderCopy(Window::_Renderer(), answer_cText, NULL, &destination);

	destination = CenterTextInsideHexagon(answer_d, answer_dText);
	SDL_RenderCopy(Window::_Renderer(), answer_dText, NULL, &destination);*/
}

unsigned int InGameScene::Hit(SDL_Point mousePos)
{
	// TODO
	return 1;
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
	/*questionText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._Question(), "question");
	answer_aText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._A(), "answer");
	answer_bText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._B(), "answer");
	answer_cText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._C(), "answer");
	answer_dText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._D(), "answer");*/

	get<SDL_Texture*>(question) = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._Question(), "question");

	// Answer A
	get<SDL_Texture*>(buttons[0]) = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._A(), "answer");

	// Answer B
	get<SDL_Texture*>(buttons[1]) = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._B(), "answer");

	// Answer C
	get<SDL_Texture*>(buttons[2]) = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._C(), "answer");

	// Answer D
	get<SDL_Texture*>(buttons[3]) = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._D(), "answer");
}

void InGameScene::Invoke(int index) const
{
	get<function<void()>>(buttons[index])();
}