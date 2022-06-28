#include "megjelenites.h"

void Jatek::terminateRunning()
{
	running = false;
	delete[] arr;
}

void Megjelenites::freeResources(Jatek& game)
{
	for(int i = 0; i < 38; ++i)
	{
		SDL_DestroyTexture(textures[i]);
	}
	SDL_DestroyTexture(rombusz); SDL_DestroyTexture(atext); SDL_DestroyTexture(btext); SDL_DestroyTexture(ctext); SDL_DestroyTexture(dtext);
	if(current_screen == "PRIZING" || (current_screen == "INGAME" && didtheplayerlose == true))
	{
		SDL_DestroyTexture(nyeremeny); 
	}
	if(current_screen == "INGAME" && game.getKozonseg().wasItUsed() == true) 
	{
		SDL_DestroyTexture(perc1);
		SDL_DestroyTexture(perc2);
		SDL_DestroyTexture(perc3);
		SDL_DestroyTexture(perc4);
	}
	SDL_DestroyTexture(done_questions);
	Mix_FreeMusic(firstfive); Mix_FreeMusic(secondfive); Mix_FreeMusic(thirdfive);
	Mix_FreeChunk(fiftyfifty); Mix_FreeChunk(audience); Mix_FreeChunk(phone); Mix_FreeChunk(pause); 
	Mix_FreeChunk(lesplay); Mix_FreeChunk(ga); Mix_FreeChunk(ba); Mix_FreeChunk(fa);
	TTF_CloseFont(font);
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
