#include "megjelenites.h"

void Megjelenites::mouseclick(Jatek& game, Adatok& data, Megjelenites& display)
{
	if(event.button.button == SDL_BUTTON_LEFT)
	{
		if(current_screen == "MAINMENU")
		{
			switch(valto)
			{
				case 1:
					current_screen = "INGAME";
					starttime = SDL_GetTicks();
					Mix_HaltChannel(-1);
					Mix_PlayChannel(-1, lesplay, 0);
					Mix_FadeInMusic(firstfive, -1, 8000);
					new_game(game, data);
					valto = 0;
					break;
				case 2:
					current_screen = "OPTIONS";
					valto = 0;
					break;
				case 3:
					game.terminateRunning();
					freeResources(game);
					break;
			}
		}
		else if(current_screen == "OPTIONS")
		{
			switch(valto)
			{
				case 1:
					current_screen = "HOWTOPLAY";
					valto = 0;
					break;
				case 2:
					current_screen = "RESOLUTIONS";
					valto = 0;
					break;
				case 3:
					if(fullscreen)
					{
						SDL_SetWindowFullscreen(window, 0);
						fullscreen = false;
					}
					else
					{
						SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
						fullscreen = true;
					}
					break;
				case 4:
					current_screen = "MAINMENU";
					valto = 0;
					break;
			}
		}
		else if(current_screen == "HOWTOPLAY")
		{
			if(valto == 1) { current_screen = "OPTIONS"; }
		}
		else if(current_screen == "RESOLUTIONS")
		{
			switch(valto)
			{
				case 1:
					if(resolution != 1)
					{
						SCREEN_WIDTH = 640;
						SCREEN_HEIGHT = 480;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1920.0 / SCREEN_WIDTH; szorzo2 = 1080.0 / SCREEN_HEIGHT;
						changeForResolution();
						resolution = 1;
					}
					break;
				case 2:
					if(resolution != 2)
					{
						SCREEN_WIDTH = 800;
						SCREEN_HEIGHT = 600;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1920.0 / SCREEN_WIDTH; szorzo2 = 1080.0 / SCREEN_HEIGHT;
						changeForResolution();
						resolution = 2;
					}
					break;
				case 3:
					if(resolution != 3)
					{
						SCREEN_WIDTH = 1024;
						SCREEN_HEIGHT = 768;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1920.0 / SCREEN_WIDTH; szorzo2 = 1080.0 / SCREEN_HEIGHT;
						changeForResolution();
						resolution = 3;
					}
					break;
				case 4:
					if(resolution != 4)
					{
						SCREEN_WIDTH = 1280;
						SCREEN_HEIGHT = 720;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1920.0 / SCREEN_WIDTH; szorzo2 = 1080.0 / SCREEN_HEIGHT;
						changeForResolution();
						resolution = 4;
					}
					break;
				case 5:
					if(resolution != 5)
					{
						SCREEN_WIDTH = 1366;
						SCREEN_HEIGHT = 768;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1920.0 / SCREEN_WIDTH; szorzo2 = 1080.0 / SCREEN_HEIGHT;
						changeForResolution();
						resolution = 5;
					}
					break;
				case 6:
					if(resolution != 6)
					{
						SCREEN_WIDTH = 1600;
						SCREEN_HEIGHT = 900;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1920.0 / SCREEN_WIDTH; szorzo2 = 1080.0 / SCREEN_HEIGHT;
						changeForResolution();
						resolution = 6;
					}
					break;
				case 7:
					if(resolution != 7)
					{
						SCREEN_WIDTH = 1920;
						SCREEN_HEIGHT = 1080;
						SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
						dest.w = SCREEN_WIDTH; dest.h = SCREEN_HEIGHT;
						szorzo1 = 1; szorzo2 = 1;
						changeForResolution();
						resolution = 7;
					}
					break;
				case 8:
					current_screen = "OPTIONS";
					valto = 0;
					break;
			}
		}
		else if(current_screen == "INGAME")
		{
			switch(valto)
			{
				case 1:
					if(therewasananswer != true)
					{
						Mix_PlayChannel(-1, fa, 0);
						valto = 1;
						therewasananswer = true;
						starttime = SDL_GetTicks();
						if(game.getValaszok()->getAnswer(game.getValaszok(), 0) != data.solutions(game.getloc(), serial_nb))
						{
							didtheplayerlose = true;
							if(game.getValaszok()->getAnswer(game.getValaszok(), 1) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_A1.png");
							}
							else if(game.getValaszok()->getAnswer(game.getValaszok(), 2) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_A2.png");
							}
							else
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_A3.png");
							}

							if(QN / 5 == 0) { nyeremeny = texturefromstring("Jutalom: " + prices[0] + " HUF", white, 'n'); }
							else if(QN / 5 == 1) { nyeremeny = texturefromstring("Jutalom: " + prices[5] + " HUF", white, 'n'); }
							else { nyeremeny = texturefromstring("Jutalom: " + prices[10] + " HUF", white, 'n'); }
						}
					}		
					break;
				case 2:
					if(therewasananswer != true)
					{
						Mix_PlayChannel(-1, fa, 0);
						valto = 2;
						therewasananswer = true;
						starttime = SDL_GetTicks();
						if(game.getValaszok()->getAnswer(game.getValaszok(), 1) != data.solutions(game.getloc(), serial_nb))
						{
							didtheplayerlose = true;
							if(game.getValaszok()->getAnswer(game.getValaszok(), 0) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_B1.png");
							}
							else if(game.getValaszok()->getAnswer(game.getValaszok(), 2) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_B2.png");
							}
							else
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_B3.png");
							}

							if(QN / 5 == 0) { nyeremeny = texturefromstring("Jutalom: " + prices[0] + " HUF", white, 'n'); }
							else if(QN / 5 == 1) { nyeremeny = texturefromstring("Jutalom: " + prices[5] + " HUF", white, 'n'); }
							else { nyeremeny = texturefromstring("Jutalom: " + prices[10] + " HUF", white, 'n'); }
						}
					}				
					break;
				case 3:
					if(therewasananswer != true)
					{
						Mix_PlayChannel(-1, fa, 0);
						valto = 3;
						therewasananswer = true;
						starttime = SDL_GetTicks();
						if(game.getValaszok()->getAnswer(game.getValaszok(), 2) != data.solutions(game.getloc(), serial_nb))
						{
							didtheplayerlose = true;
							if(game.getValaszok()->getAnswer(game.getValaszok(), 0) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_C1.png");
							}
							else if(game.getValaszok()->getAnswer(game.getValaszok(), 1) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_C2.png");
							}
							else
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_C3.png");
							}

							if(QN / 5 == 0) { nyeremeny = texturefromstring("Jutalom: " + prices[0] + " HUF", white, 'n'); }
							else if(QN / 5 == 1) { nyeremeny = texturefromstring("Jutalom: " + prices[5] + " HUF", white, 'n'); }
							else { nyeremeny = texturefromstring("Jutalom: " + prices[10] + " HUF", white, 'n'); }
						}
					}				
					break;
				case 4:
					if(therewasananswer != true)
					{
						Mix_PlayChannel(-1, fa, 0);
						valto = 4;
						therewasananswer = true;
						starttime = SDL_GetTicks();
						if(game.getValaszok()->getAnswer(game.getValaszok(), 3) != data.solutions(game.getloc(), serial_nb))
						{
							didtheplayerlose = true;
							if(game.getValaszok()->getAnswer(game.getValaszok(), 0) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_D1.png");
							}
							else if(game.getValaszok()->getAnswer(game.getValaszok(), 1) == data.solutions(game.getloc(), serial_nb))
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_D2.png");
							}
							else
							{
								textures[38] = IMG_LoadTexture(renderer, "textures/ingame_incorrect_D3.png");
							}

							if(QN / 5 == 0) { nyeremeny = texturefromstring("Jutalom: " + prices[0] + " HUF", white, 'n'); }
							else if(QN / 5 == 1) { nyeremeny = texturefromstring("Jutalom: " + prices[5] + " HUF", white, 'n'); }
							else { nyeremeny = texturefromstring("Jutalom: " + prices[10] + " HUF", white, 'n'); }
						}
					}				
					break;
				case 5:
					if(game.getFelezo().wasItUsed() != true && allow_control2 == true)
					{
						allow_control2 = false;
						Mix_PlayChannel(-1, fiftyfifty, 0);
						game.getFelezo().halving(game, data, display);
						game.getFelezo().halving(game, data, display, game.getFelezo().getFSHI());
						game.getFelezo().setCurrentRound(display);
						game.getFelezo().setUsedTrue();
					}
					break;
				case 6:
					if(game.getTelefonos().wasItUsed() != true && allow_control2 == true)
					{
						allow_control2 = false;
						Mix_PlayChannel(-1, phone, 0);
						game.getTelefonos().givehint(game, data, display);
						setCallParameters(game);
						game.getTelefonos().setCurrentRound(display);
						game.getTelefonos().setUsedTrue();
						starttime = SDL_GetTicks();
					}
					break;
				case 7:
					if(game.getKozonseg().wasItUsed() != true && allow_control2 == true)
					{
						allow_control2 = false;
						Mix_PlayChannel(-1, audience, 0);
						game.getKozonseg().createchart(game, data, display);
						setAudienceParameters(game);
						game.getKozonseg().setCurrentRound(display);
						game.getKozonseg().setUsedTrue();
						starttime = SDL_GetTicks();
					}
					break;
				case 9:
					if(didtheplayerlose == false) { nyeremeny = texturefromstring("Jutalom: " + prices[QN] + " HUF", white, 'n'); }
					Mix_PlayChannel(-1, pause, 0);
					Mix_HaltMusic();
					current_screen = "PRIZING";
					valto = 0;			
					break;
			}
		}
		else
		{
			Mix_HaltChannel(-1);
			Mix_HaltMusic();
			current_screen = "MAINMENU";
			if(QN != 15)
			{
				SDL_DestroyTexture(curr_a1);
				SDL_DestroyTexture(curr_a2);
				SDL_DestroyTexture(curr_a3);
				SDL_DestroyTexture(curr_a4);
				SDL_DestroyTexture(current_question);
			}		
			SDL_DestroyTexture(nyeremeny);
			if(game.getKozonseg().wasItUsed() == true)
			{
				SDL_DestroyTexture(perc1);
				SDL_DestroyTexture(perc2);
				SDL_DestroyTexture(perc3);
				SDL_DestroyTexture(perc4);
			}
		}
	}
}
