#include "megjelenites.h"

void Megjelenites::screen_renderer(Jatek& game, Adatok& data)
{
	if(current_screen == "MAINMENU")
	{
		SDL_RenderCopy(renderer, textures[valto], NULL, &dest);
	}
	else if(current_screen == "OPTIONS")
	{
		if(fullscreen)
		{
			SDL_RenderCopy(renderer, textures[33 + valto], NULL, &dest);
		}
		else
		{
			SDL_RenderCopy(renderer, textures[valto + 4], NULL, &dest);
		}		
	}
	else if(current_screen == "HOWTOPLAY")
	{
		SDL_RenderCopy(renderer, textures[27 + valto], NULL, &dest);
	}
	else if(current_screen == "RESOLUTIONS")
	{
		SDL_RenderCopy(renderer, textures[valto + 9], NULL, &dest);
	}
	else if(current_screen == "INGAME")
	{
		screen_renderer_ING(game, data);
	}
	else
	{
		SDL_RenderCopy(renderer, textures[26], NULL, &dest);
		SDL_RenderCopy(renderer, nyeremeny, NULL, &ny_poz);
	}
}

void Megjelenites::screen_renderer_ING(Jatek& game, Adatok& data)
{
	render_background(game, data);
	renderX(game);
	render_cq_designate();
	render_done_questions();

	if(allow_control == false)
	{
		int delay = 0;
		(QN % 5 == 0) ? delay = 5000 : delay = 2000;
		if((int)SDL_GetTicks() > (starttime + delay)) { render_question(); }
		if((int)SDL_GetTicks() > (starttime + delay + 2000)) { render_answerA(); }
		if((int)SDL_GetTicks() > (starttime + delay + 4000)) { render_answerB(); }
		if((int)SDL_GetTicks() > (starttime + delay + 6000)) { render_answerC(); }
		if((int)SDL_GetTicks() > (starttime + delay + 8000)) { render_answerD(); allow_control = true; }
	}
	else
	{
		render_question(); render_answerA(); render_answerB(); render_answerC(); render_answerD();
		renderSegitsegek(game);
	}
}

void Megjelenites::render_background(Jatek& game, Adatok& data)
{
	if(valto == 0 || valto == 5 || valto == 6 || valto == 7 || (valto == 9 && didtheplayerlose == false))
	{
		SDL_RenderCopy(renderer, textures[18], NULL, &dest); 
	}
	else if(valto == 1 && game.getFelezo().getCurrentRound() != QN || (valto == 1 && game.getFelezo().getFSHI() != 0 && game.getFelezo().getSSHI() != 0))
	{
		if(therewasananswer == false) { SDL_RenderCopy(renderer, textures[19], NULL, &dest); }
		else 
		{
			if(SDL_GetTicks() < (unsigned int)(starttime + 2000))
			{
				SDL_RenderCopy(renderer, textures[19], NULL, &dest);
			}
			else if(SDL_GetTicks() < (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ga, 0); }
					SDL_RenderCopy(renderer, textures[29], NULL, &dest);
					playedsound = true;
				}
				else
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ba, 0); }
					Mix_HaltMusic();
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					playedsound = true;
				}				
			}
			else if(SDL_GetTicks() >= (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					valto = 0;
					++QN;
					if(QN == 5) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(secondfive, -1, 8000); }
					else if(QN == 10) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(thirdfive, -1, 8000); }
					therewasananswer = false;
					nextquestion(game, data);
				}
				else
				{
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					valto = 9;
				}
			}
		}
	}
	else if(valto == 2 && game.getFelezo().getCurrentRound() != QN || (valto == 2 && game.getFelezo().getFSHI() != 1 && game.getFelezo().getSSHI() != 1))
	{
		if(therewasananswer == false) { SDL_RenderCopy(renderer, textures[20], NULL, &dest); }
		else
		{
			if(SDL_GetTicks() < (unsigned int)(starttime + 2000))
			{
				SDL_RenderCopy(renderer, textures[20], NULL, &dest);
			}
			else if(SDL_GetTicks() < (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ga, 0); }
					SDL_RenderCopy(renderer, textures[30], NULL, &dest);
					playedsound = true;
				}
				else
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ba, 0); }
					Mix_HaltMusic();
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					playedsound = true;
				}
			}
			else if(SDL_GetTicks() >= (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					++QN;
					if(QN == 5) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(secondfive, -1, 8000); }
					else if(QN == 10) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(thirdfive, -1, 8000); }
					valto = 0;
					therewasananswer = false;
					nextquestion(game, data);
				}
				else
				{
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					valto = 9;
				}
			}
		}
	}
	else if(valto == 3 && game.getFelezo().getCurrentRound() != QN || (valto == 3 && game.getFelezo().getFSHI() != 2 && game.getFelezo().getSSHI() != 2))
	{
		if(therewasananswer == false) { SDL_RenderCopy(renderer, textures[21], NULL, &dest); }
		else
		{
			if(SDL_GetTicks() < (unsigned int)(starttime + 2000))
			{
				SDL_RenderCopy(renderer, textures[21], NULL, &dest);
			}
			else if(SDL_GetTicks() < (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ga, 0); }
					SDL_RenderCopy(renderer, textures[31], NULL, &dest);
					playedsound = true;
				}
				else
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ba, 0); }
					Mix_HaltMusic();
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					playedsound = true;
				}
			}
			else if(SDL_GetTicks() >= (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					++QN;
					if(QN == 5) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(secondfive, -1, 8000); }
					else if(QN == 10) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(thirdfive, -1, 8000); }
					valto = 0;
					therewasananswer = false;
					nextquestion(game, data);
				}
				else
				{
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					valto = 9;
				}
			}
		}
	}
	else if(valto == 4 && game.getFelezo().getCurrentRound() != QN || (valto == 4 && game.getFelezo().getFSHI() != 3 && game.getFelezo().getSSHI() != 3)) 
	{
		if(therewasananswer == false) { SDL_RenderCopy(renderer, textures[22], NULL, &dest); }
		else
		{
			if(SDL_GetTicks() < (unsigned int)starttime + 2000)
			{
				SDL_RenderCopy(renderer, textures[22], NULL, &dest);
			}
			else if(SDL_GetTicks() < (unsigned int)starttime + 4000)
			{
				if(didtheplayerlose == false)
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ga, 0); }
					SDL_RenderCopy(renderer, textures[32], NULL, &dest);
					playedsound = true;
				}
				else
				{
					if(playedsound != true) { Mix_PlayChannel(-1, ba, 0); }
					Mix_HaltMusic();
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					playedsound = true;
				}
			}
			else if(SDL_GetTicks() >= (unsigned int)(starttime + 4000))
			{
				if(didtheplayerlose == false)
				{
					++QN;
					if(QN == 5) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(secondfive, -1, 8000); }
					else if(QN == 10) { Mix_HaltMusic(); Mix_PlayChannel(-1, pause, 0); Mix_FadeInMusic(thirdfive, -1, 8000); }
					valto = 0;
					therewasananswer = false;
					nextquestion(game, data);
				}
				else
				{
					SDL_RenderCopy(renderer, textures[38], NULL, &dest);
					valto = 9;
				}
			}
		}
	}
	else if(valto == 9)
	{
		SDL_RenderCopy(renderer, textures[38], NULL, &dest);
	}
}

void Megjelenites::render_question()
{
	SDL_RenderCopy(renderer, current_question, NULL, &q1);  
}

void Megjelenites::render_answerA()
{
	SDL_RenderCopy(renderer, atext, NULL, &a);
	SDL_RenderCopyEx(renderer, rombusz, NULL, &r1, 45.0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, curr_a1, NULL, &a1);
}

void Megjelenites::render_answerB()
{
	SDL_RenderCopy(renderer, btext, NULL, &b);
	SDL_RenderCopyEx(renderer, rombusz, NULL, &r2, 45.0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, curr_a2, NULL, &a2);	
}

void Megjelenites::render_answerC()
{
	SDL_RenderCopy(renderer, ctext, NULL, &c);
	SDL_RenderCopyEx(renderer, rombusz, NULL, &r3, 45.0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, curr_a3, NULL, &a3);	
}

void Megjelenites::render_answerD()
{
	SDL_RenderCopy(renderer, dtext, NULL, &d);
	SDL_RenderCopyEx(renderer, rombusz, NULL, &r4, 45.0, NULL, SDL_FLIP_NONE);
	SDL_RenderCopy(renderer, curr_a4, NULL, &a4);		
}

void Megjelenites::renderX(Jatek& game)
{
	if(game.getFelezo().wasItUsed() == true) { SDL_RenderCopy(renderer, textures[23], NULL, &x1); }
	if(game.getTelefonos().wasItUsed() == true) 
	{
		if(SDL_GetTicks() >= (unsigned int)(starttime + 4000) || phonedelaywasused == true)
		{
			SDL_RenderCopy(renderer, textures[23], NULL, &x2);
		}	 
	}
	if(game.getKozonseg().wasItUsed() == true) 
	{
		if(SDL_GetTicks() >= (unsigned int)(starttime + 4000) || audiencedelaywasused == true)
		{
			SDL_RenderCopy(renderer, textures[23], NULL, &x3);
		}	 
	}
}

void Megjelenites::renderSegitsegek(Jatek& game)
{
	if(game.getFelezo().wasItUsed() == true && game.getFelezo().getCurrentRound() == QN)
	{
		if(halveningdelaywasused == false) { allow_control2 = true; }
		halveningdelaywasused = true;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &hider_dest1);
		SDL_RenderFillRect(renderer, &hider_dest2);
	}
	if(game.getTelefonos().wasItUsed() == true && game.getTelefonos().getCurrentRound() == QN)
	{
		if(SDL_GetTicks() >= (unsigned int)(starttime + 4000) || phonedelaywasused == true)
		{
			if(phonedelaywasused == false) { allow_control2 = true; }
			phonedelaywasused = true;
			SDL_RenderCopy(renderer, textures[24], NULL, &calldest);
			if(game.getTelefonos().getLetter() == 'A') { SDL_RenderCopy(renderer, atext, NULL, &calldestletter); }
			else if(game.getTelefonos().getLetter() == 'B') { SDL_RenderCopy(renderer, btext, NULL, &calldestletter); }
			else if(game.getTelefonos().getLetter() == 'C') { SDL_RenderCopy(renderer, ctext, NULL, &calldestletter); }
			else { SDL_RenderCopy(renderer, dtext, NULL, &calldestletter); }
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &hider_dest3);
		}
	}
	if(game.getKozonseg().wasItUsed() == true && game.getKozonseg().getCurrentRound() == QN)
	{
		if(SDL_GetTicks() >= (unsigned int)(starttime + 4000) || audiencedelaywasused == true)
		{
			if(audiencedelaywasused == false) { allow_control2 = true; }
			audiencedelaywasused = true;
			SDL_RenderCopy(renderer, textures[25], NULL, &audiencedest);
			SDL_SetRenderDrawColor(renderer, 185, 92, 230, 255);
			SDL_RenderFillRect(renderer, &column_a);
			SDL_RenderFillRect(renderer, &column_b);
			SDL_RenderFillRect(renderer, &column_c);
			SDL_RenderFillRect(renderer, &column_d);
			SDL_RenderCopy(renderer, perc1, NULL, &perc1_poz);
			SDL_RenderCopy(renderer, perc2, NULL, &perc2_poz);
			SDL_RenderCopy(renderer, perc3, NULL, &perc3_poz);
			SDL_RenderCopy(renderer, perc4, NULL, &perc4_poz);		
		}
	}
}
