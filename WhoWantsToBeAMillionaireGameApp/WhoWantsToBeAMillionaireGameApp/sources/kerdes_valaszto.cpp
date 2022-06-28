#include "megjelenites.h"

int Megjelenites::question_selector(Jatek& game, Adatok& data)
{
    char current_diff;

    if(QN < 5)
    {
        current_diff = 'E';
    }
    else if(QN < 10)
    {
        current_diff = 'M';
    }
    else
    {
        current_diff = 'H';
    }

    string tema = game.catChoosing(data.Random(0, (game.getCatLength() - 1)));
    bool b;
    int random;

    do
    {
        b = true;
        random = data.Random(data.getfirstoccurrence(tema, game.getloc(), game.getlen(game.getCatLength())), data.getfirstoccurrence(tema, game.getloc(), game.getlen(game.getCatLength())) + game.getlen(game.getCatIdx(tema)) - 1);

        if(data.difficulty(game.getloc(), random) != current_diff)
        {
            b = false;
        }

        if(b != false)
        {
            for(int i = 0; i < QN; ++i)
            {
                if(tema == game.getKerdesek()->getQuestionC(game.getKerdesek(), i) && random == game.getKerdesek()->getQuestionN(game.getKerdesek(), i))
                {
                    b = false;
                    break;
                }
            }
        }
    } while(b == false);

    game.getKerdesek()->setQuestion(game.getKerdesek(), QN, random, tema);
    return random;
}
