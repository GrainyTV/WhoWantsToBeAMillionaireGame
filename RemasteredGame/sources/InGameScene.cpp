#include "InGameScene.hpp"
#include "Window.hpp"
#include "Game.hpp"

InGameScene::InGameScene()
:
question({ Vec2(340, 705), Vec2(390, 650), Vec2(390, 760), Vec2(1530, 650), Vec2(1530, 760), Vec2(1580, 705) }),
answer_a({ Vec2(340, 845), Vec2(390, 790), Vec2(390, 900), Vec2(896, 790), Vec2(896, 900), Vec2(946, 845) }),
answer_b({ Vec2(974, 845), Vec2(1024, 790), Vec2(1024, 900), Vec2(1530, 790), Vec2(1530, 900), Vec2(1580, 845) }),
answer_c({ Vec2(340, 985), Vec2(390, 930), Vec2(390, 1040), Vec2(896, 930), Vec2(896, 1040), Vec2(946, 985) }),
answer_d({ Vec2(974, 985), Vec2(1024, 930), Vec2(1024, 1040), Vec2(1530, 930), Vec2(1530, 1040), Vec2(1580, 985) })
{
	if(CheckTextboxPositionValidity() == false)
	{
		throw runtime_error("The 4 answer and 1 question textboxes do not line up.");
	}
}

void InGameScene::Draw() const
{
	question.Draw();
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
	SDL_RenderCopy(Window::_Renderer(), answer_dText, NULL, &destination);
}

unsigned int InGameScene::Hit(SDL_Point mousePos)
{
	// TODO
	return 1;
}

bool InGameScene::CheckTextboxPositionValidity() const
{
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
	bool verticalScaled = question == aCombinedb.Translate(0, -140);

	return  (vertical && horizontal && diagonal && verticalScaled) ? true : false;
}

void InGameScene::InitiateNewGame()
{
	currentGame = make_unique<NewGame>((*Game::Instance()).GenerateNewGame());
	questionText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._Question(), "question");
	answer_aText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._A(), "answer");
	answer_bText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._B(), "answer");
	answer_cText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._C(), "answer");
	answer_dText = CreateTextureFromText((*currentGame.get()).ThisRoundsData()._D(), "answer");
}