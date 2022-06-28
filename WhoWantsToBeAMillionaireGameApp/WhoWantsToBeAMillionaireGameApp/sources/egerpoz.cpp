#include "megjelenites.h"

void Megjelenites::mousepos(Jatek& game)
{
	SDL_GetMouseState(&x, &y);

	if(current_screen == "MAINMENU")
	{
		if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (650 / szorzo2) && y <= (760 / szorzo2))
		{
			valto = 1;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (795 / szorzo2) && y <= (905 / szorzo2))
		{
			valto = 2;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (940 / szorzo2) && y <= (1050 / szorzo2))
		{
			valto = 3;
		}
		else
		{
			valto = 0;
		}
	}
	else if(current_screen == "HOWTOPLAY")
	{
		if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (805 / szorzo2) && y <= (915 / szorzo2))
		{
			valto = 1;
		}
		else
		{
			valto = 0;
		}
	}
	else if(current_screen == "OPTIONS")
	{
		if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (370 / szorzo2) && y <= (480 / szorzo2))
		{
			valto = 1;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (515 / szorzo2) && y <= (625 / szorzo2))
		{
			valto = 2;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (660 / szorzo2) && y <= (770 / szorzo2))
		{
			valto = 3;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (805 / szorzo2) && y <= (915 / szorzo2))
		{
			valto = 4;
		}
		else
		{
			valto = 0;
		}
	}
	else if(current_screen == "RESOLUTIONS")
	{
		if(x >= (340 / szorzo1) && x <= (925 / szorzo1) && y >= (235 / szorzo2) && y <= (345 / szorzo2))
		{
			valto = 1;
		}
		else if(x >= (995 / szorzo1) && x <= (1580 / szorzo1) && y >= (235 / szorzo2) && y <= (345 / szorzo2))
		{
			valto = 2;
		}
		else if(x >= (340 / szorzo1) && x <= (925 / szorzo1) && y >= (380 / szorzo2) && y <= (490 / szorzo2))
		{
			valto = 3;
		}
		else if(x >= (995 / szorzo1) && x <= (1580 / szorzo1) && y >= (380 / szorzo2) && y <= (490 / szorzo2))
		{
			valto = 4;
		}
		else if(x >= (340 / szorzo1) && x <= (925 / szorzo1) && y >= (525 / szorzo2) && y <= (635 / szorzo2))
		{
			valto = 5;
		}
		else if(x >= (995 / szorzo1) && x <= (1580 / szorzo1) && y >= (525 / szorzo2) && y <= (635 / szorzo2))
		{
			valto = 6;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (670 / szorzo2) && y <= (780 / szorzo2))
		{
			valto = 7;
		}
		else if(x >= (665 / szorzo1) && x <= (1260 / szorzo1) && y >= (805 / szorzo2) && y <= (915 / szorzo2))
		{
			valto = 8;
		}
		else
		{
			valto = 0;
		}
	}
	else if(current_screen == "INGAME")
	{
		if(x >= (389 / szorzo1) && x <= (878 / szorzo1) && y >= (799 / szorzo2) && y <= (903 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			if(game.getFelezo().getCurrentRound() != QN || (game.getFelezo().getFSHI() != 0 && game.getFelezo().getSSHI() != 0))
			{
				valto = 1;
			}
		}
		else if(x >= (1041 / szorzo1) && x <= (1530 / szorzo1) && y >= (799 / szorzo2) && y <= (903 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			if(game.getFelezo().getCurrentRound() != QN || (game.getFelezo().getFSHI() != 1 && game.getFelezo().getSSHI() != 1))
			{
				valto = 2;
			}
		}
		else if(x >= (389 / szorzo1) && x <= (878 / szorzo1) && y >= (942 / szorzo2) && y <= (1046 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			if(game.getFelezo().getCurrentRound() != QN || (game.getFelezo().getFSHI() != 2 && game.getFelezo().getSSHI() != 2))
			{
				valto = 3;
			}
		}
		else if(x >= (1041 / szorzo1) && x <= (1530 / szorzo1) && y >= (942 / szorzo2) && y <= (1046 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			if(game.getFelezo().getCurrentRound() != QN || (game.getFelezo().getFSHI() != 3 && game.getFelezo().getSSHI() != 3))
			{
				valto = 4;
			}
		}
		else if(x >= (1262 / szorzo1) && x <= (1336 / szorzo1) && y >= (50 / szorzo2) && y <= (89 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			valto = 5;
		}
		else if(x >= (1347 / szorzo1) && x <= (1422 / szorzo1) && y >= (50 / szorzo2) && y <= (89 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			valto = 6;
		}
		else if(x >= (1433 / szorzo1) && x <= (1507 / szorzo1) && y >= (50 / szorzo2) && y <= (89 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			valto = 7;
		}
		else if(x >= (1847 / szorzo1) && x <= (1920 / szorzo1) && y >= (0 / szorzo2) && y <= (41 / szorzo2) && allow_control == true && therewasananswer == false)
		{
			valto = 9;
		}
		else if(allow_control == true && therewasananswer == false)
		{
			valto = 0;
		}
	}
}

