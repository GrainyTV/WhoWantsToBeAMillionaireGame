#include "megjelenites.h"

void Megjelenites::nextquestion(Jatek& game, Adatok& data)
{
	if(QN > 0)
	{
		SDL_DestroyTexture(curr_a1);
		SDL_DestroyTexture(curr_a2);
		SDL_DestroyTexture(curr_a3);
		SDL_DestroyTexture(curr_a4);
		SDL_DestroyTexture(current_question);
	}

	if(QN == 15)
	{
		nyeremeny = texturefromstring("Jutalom: " + prices[QN] + " HUF", white, 'n');
		Mix_PlayChannel(-1, pause, 0);
		Mix_HaltMusic();
		current_screen = "PRIZING";
		valto = 0;
	}
	else
	{
		allow_control = false;
		serial_nb = question_selector(game, data);
		current_question = texturefromstring(data.questions(game.getloc(), serial_nb), white, 'Q');

		game.getValaszok()->setAnswer(game.getValaszok(), 0, data.answer1(game.getloc(), serial_nb));
		game.getValaszok()->setAnswer(game.getValaszok(), 1, data.answer2(game.getloc(), serial_nb));
		game.getValaszok()->setAnswer(game.getValaszok(), 2, data.answer3(game.getloc(), serial_nb));
		game.getValaszok()->setAnswer(game.getValaszok(), 3, data.answer4(game.getloc(), serial_nb));

		for(int i = 3; i >= 0; --i)
		{
			int poz = data.Random(0, i);

			if(poz != i)
			{
				string tmp = game.getValaszok()->getAnswer(game.getValaszok(), poz);
				game.getValaszok()->setAnswer(game.getValaszok(), poz, game.getValaszok()->getAnswer(game.getValaszok(), i));
				game.getValaszok()->setAnswer(game.getValaszok(), i, tmp);
			}
		}

		curr_a1 = texturefromstring(game.getValaszok()->getAnswer(game.getValaszok(), 0), white, 'A');
		curr_a2 = texturefromstring(game.getValaszok()->getAnswer(game.getValaszok(), 1), white, 'B');
		curr_a3 = texturefromstring(game.getValaszok()->getAnswer(game.getValaszok(), 2), white, 'C');
		curr_a4 = texturefromstring(game.getValaszok()->getAnswer(game.getValaszok(), 3), white, 'D');

		starttime = SDL_GetTicks();
		playedsound = false;
	}	
}
