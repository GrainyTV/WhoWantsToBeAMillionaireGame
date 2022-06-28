#include "megjelenites.h"

void Megjelenites::new_game(Jatek& game, Adatok& data)
{
	QN = 0;
	previous = 0;
	didtheplayerlose = false;
	therewasananswer = false;
	audiencedelaywasused = false;
	phonedelaywasused = false;
	halveningdelaywasused = false;
	cq_designate.y = (int)(518 / szorzo2);

	for(int i = 0; i < 15; ++i)
	{
		game.getKerdesek()->setQuestion(game.getKerdesek(), i, 0, "History");
	}

	game.getFelezo().resetHelp();
	game.getFelezo().setFSHI(-1);
	game.getFelezo().setSSHI(-1);
	game.getTelefonos().resetHelp();
	game.getKozonseg().resetHelp();

	nextquestion(game, data);
}
