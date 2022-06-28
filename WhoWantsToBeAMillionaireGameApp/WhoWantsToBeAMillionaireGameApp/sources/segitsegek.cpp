#include "megjelenites.h"
#include "segitsegek.h"

bool& Segitsegek::wasItUsed()
{
	return isAlreadyUsed;
}

void Segitsegek::setUsedTrue()
{
	isAlreadyUsed = true;
}

void Segitsegek::setCurrentRound(Megjelenites& display)
{
	current_round = display.getQN();
}

int Segitsegek::getCurrentRound()
{
	return current_round;
}

void Segitsegek::resetHelp()
{
	isAlreadyUsed = false;
}

void Felezo::halving(Jatek& game, Adatok& data, Megjelenites& display, int previous)
{
	int locked = 0;
	int idx = 0;
	bool b;

	for(int i = 0; i < 4; ++i)
	{
		if(game.getValaszok()->getAnswer(game.getValaszok(), i) == data.solutions(game.getloc(), display.getserial_nb()))
		{	
			locked = i;
			break;
		}
	}

	do
	{
		b = true;
		idx = data.Random(0, 3);

		if(idx == locked)
		{
			b = false;
		}
		else if(idx == previous)
		{
			b = false;
		}

	} while(b == false);

	(previous == -1) ? setFSHI(idx) : setSSHI(idx);

	display.setHiders(idx, previous); 
}

int Felezo::getFSHI()
{
	return firstScreenHiderIdx;
}

int Felezo::getSSHI()
{
	return secondScreenHiderIdx;
}

void Felezo::setFSHI(int idx)
{
	firstScreenHiderIdx = idx;
}

void Felezo::setSSHI(int idx)
{
	secondScreenHiderIdx = idx;
}

void Telefonos::givehint(Jatek& game, Adatok& data, Megjelenites& display)
{
	int idx = -1;

	for(int i = 0; i < 4; ++i)
	{
		if(game.getValaszok()->getAnswer(game.getValaszok(), i) == data.solutions(game.getloc(), display.getserial_nb()))
		{
			idx = i;
			break;
		}
	}

	if(idx == 0) { sol_letter = 'A'; }
	else if(idx == 1) { sol_letter = 'B'; }
	else if(idx == 2) { sol_letter = 'C'; }
	else { sol_letter = 'D'; }
}

char Telefonos::getLetter()
{
	return sol_letter;
}

void Kozonseg::createchart(Jatek& game, Adatok& data, Megjelenites& display)
{
	int idx = -1;

	for(int i = 0; i < 4; ++i)
	{
		if(game.getValaszok()->getAnswer(game.getValaszok(), i) == data.solutions(game.getloc(), display.getserial_nb()))
		{
			idx = i;
			break;
		}
	}

	if(game.getFelezo().wasItUsed() == true && game.getFelezo().getCurrentRound() == display.getQN())
	{
		if(idx == 0) 
		{
			sz_A = data.Random(61, 97);
			if((game.getFelezo().getFSHI() == 1 && game.getFelezo().getSSHI() == 2) || (game.getFelezo().getFSHI() == 2 && game.getFelezo().getSSHI() == 1))
			{			
				sz_B = 0;
				sz_C = 0;
				sz_D = 100 - sz_A;
			}
			else if((game.getFelezo().getFSHI() == 1 && game.getFelezo().getSSHI() == 3) || (game.getFelezo().getFSHI() == 3 && game.getFelezo().getSSHI() == 1))
			{
				sz_B = 0;
				sz_C = 100 - sz_A;		
				sz_D = 0;
			}
			else
			{
				sz_B = 100 - sz_A;
				sz_C = 0;
				sz_D = 0;
			}
		}
		else if(idx == 1)
		{
			sz_B = data.Random(61, 97);
			if((game.getFelezo().getFSHI() == 0 && game.getFelezo().getSSHI() == 2) || (game.getFelezo().getFSHI() == 2 && game.getFelezo().getSSHI() == 0))
			{
				sz_A = 0;
				sz_C = 0;
				sz_D = 100 - sz_B;			
			}
			else if((game.getFelezo().getFSHI() == 0 && game.getFelezo().getSSHI() == 3) || (game.getFelezo().getFSHI() == 3 && game.getFelezo().getSSHI() == 0))
			{
				sz_A = 0;
				sz_C = 100 - sz_B;
				sz_D = 0;			
			}
			else
			{
				sz_A = 100 - sz_B;			
				sz_C = 0;
				sz_D = 0;
			}
		}
		else if(idx == 2)
		{
			sz_C = data.Random(61, 97);
			if((game.getFelezo().getFSHI() == 0 && game.getFelezo().getSSHI() == 1) || (game.getFelezo().getFSHI() == 1 && game.getFelezo().getSSHI() == 0))
			{
				sz_A = 0;	
				sz_B = 0;
				sz_D = 100 - sz_C;
			}
			else if((game.getFelezo().getFSHI() == 0 && game.getFelezo().getSSHI() == 3) || (game.getFelezo().getFSHI() == 3 && game.getFelezo().getSSHI() == 0))
			{
				sz_A = 0;
				sz_B = 100 - sz_C;
				sz_D = 0;				
			}
			else
			{
				sz_A = 100 - sz_C;
				sz_B = 0;
				sz_D = 0;
			}
		}
		else
		{
			sz_D = data.Random(61, 97);
			if((game.getFelezo().getFSHI() == 0 && game.getFelezo().getSSHI() == 1) || (game.getFelezo().getFSHI() == 1 && game.getFelezo().getSSHI() == 0))
			{
				sz_A = 0;
				sz_B = 0;	
				sz_C = 100 - sz_D;
			}
			else if((game.getFelezo().getFSHI() == 0 && game.getFelezo().getSSHI() == 2) || (game.getFelezo().getFSHI() == 2 && game.getFelezo().getSSHI() == 0))
			{
				sz_A = 0;
				sz_B = 100 - sz_D;			
				sz_C = 0;
			}
			else
			{
				sz_A = 100 - sz_D;
				sz_B = 0;
				sz_C = 0;
			}
		}
	}
	else
	{
		if(idx == 0) { sz_A = data.Random(51, 97); sz_B = data.Random(0, 100 - (int)sz_A); sz_C = data.Random(0, 100 - (int)(sz_A + sz_B)); sz_D = 100 - (sz_A + sz_B + sz_C); }
		else if(idx == 1) { sz_B = data.Random(51, 97); sz_A = data.Random(0, 100 - (int)sz_B); sz_C = data.Random(0, 100 - (int)(sz_B + sz_A)); sz_D = 100 - (sz_B + sz_A + sz_C); }
		else if(idx == 2) { sz_C = data.Random(51, 97); sz_A = data.Random(0, 100 - (int)sz_C); sz_B = data.Random(0, 100 - (int)(sz_C + sz_A)); sz_D = 100 - (sz_C + sz_A + sz_B); }
		else { sz_D = data.Random(51, 97); sz_A = data.Random(0, 100 - (int)sz_D); sz_B = data.Random(0, 100 - (int)(sz_D + sz_A)); sz_C = 100 - (sz_D + sz_A + sz_B); }
	}

	sz_A /= 100; sz_B /= 100; sz_C /= 100; sz_D /= 100;
}

double Kozonseg::getSzorzo(int idx)
{
	if(idx == 0) { return sz_A; }
	else if(idx == 1) { return sz_B; }
	else if(idx == 2) { return sz_C; }
	else { return sz_D; }
}