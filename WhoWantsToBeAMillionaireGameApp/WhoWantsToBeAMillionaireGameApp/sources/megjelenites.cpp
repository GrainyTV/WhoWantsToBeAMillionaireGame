#include "megjelenites.h"

void Megjelenites::setserial_nb(int number)
{
	serial_nb = number;
}

int Megjelenites::getserial_nb()
{
	return serial_nb;
}

Megjelenites::Megjelenites()
{
	SCREEN_WIDTH = 640;
	SCREEN_HEIGHT = 480;
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	window = SDL_CreateWindow("Legyen on is milliomos!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	fullscreen = false;
	allow_control = false;
	allow_control2 = true;
	therewasananswer = false;
	playedsound = false;
	didtheplayerlose = false;
	audiencedelaywasused = false;
	phonedelaywasused = false;
	halveningdelaywasused = false;
	resolution = 1;
	serial_nb = -1;
	current_screen = "MAINMENU";
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Mix_Volume(-1, 5);
	Mix_VolumeMusic(5);
	dest = rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	szorzo1 = 1920.0 / SCREEN_WIDTH;
	szorzo2 = 1080.0 / SCREEN_HEIGHT;
	white = { 255, 255, 255 };
	orange = { 254, 125, 5 };
	valto = 0;
	previous = 0;
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Surface* tmp = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 254, 125, 5));
	rombusz = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	tmp = SDL_CreateRGBSurface(0, 5, 5, 32, 0, 0, 0, 0);
	SDL_FillRect(tmp, NULL, SDL_MapRGB(tmp->format, 255, 255, 255));
	done_questions = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);

	r1 = rectangle((int)(353 / szorzo1), (int)(842 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));
	r2 = rectangle((int)(1005 / szorzo1), (int)(842 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));
	r3 = rectangle((int)(353 / szorzo1), (int)(985 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));
	r4 = rectangle((int)(1005 / szorzo1), (int)(985 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));

	x1 = rectangle((int)(1257 / szorzo1), (int)(50 / szorzo2), (int)(87 / szorzo1), (int)(44 / szorzo2));
	x2 = rectangle((int)(1343 / szorzo1), (int)(50 / szorzo2), (int)(87 / szorzo1), (int)(44 / szorzo2));
	x3 = rectangle((int)(1430 / szorzo1), (int)(50 / szorzo2), (int)(87 / szorzo1), (int)(44 / szorzo2));

	calldest = rectangle((int)(383 / szorzo1), (int)(475 / szorzo2), (int)(819 / szorzo1), (int)(110 / szorzo2));
	audiencedest = rectangle((int)(383 / szorzo1), (int)(28 / szorzo2), (int)(819 / szorzo1), (int)(436 / szorzo2));

	cq_designate = rectangle((int)(1246 / szorzo1), (int)(518 / szorzo2), (int)(278 / szorzo1), (int)(21 / szorzo2));

	column_a.x = (int)(536 / szorzo1); column_a.y = (int)(audiencedest.y + 0.82 * audiencedest.h); column_a.w = (int)(45 / szorzo1);
	column_b.x = (int)(696 / szorzo1); column_b.y = (int)(audiencedest.y + 0.82 * audiencedest.h); column_b.w = (int)(45 / szorzo1);
	column_c.x = (int)(856 / szorzo1); column_c.y = (int)(audiencedest.y + 0.82 * audiencedest.h); column_c.w = (int)(45 / szorzo1);
	column_d.x = (int)(1016 / szorzo1); column_d.y = (int)(audiencedest.y + 0.82 * audiencedest.h); column_d.w = (int)(45 / szorzo1);

	atext = texturefromstring("A:", orange, 'a');
	btext = texturefromstring("B:", orange, 'b');
	ctext = texturefromstring("C:", orange, 'c');
	dtext = texturefromstring("D:", orange, 'd');

	pause = Mix_LoadWAV("audio/commercial_break.mp3");
	Mix_PlayChannel(-1, pause, 0);

	textures[0] = IMG_LoadTexture(renderer, "textures/mainmenu_default.png");
	textures[1] = IMG_LoadTexture(renderer, "textures/mainmenu_1.png");
	textures[2] = IMG_LoadTexture(renderer, "textures/mainmenu_2.png");
	textures[3] = IMG_LoadTexture(renderer, "textures/mainmenu_3.png");
	textures[4] = IMG_LoadTexture(renderer, "textures/options_default.png");
	textures[5] = IMG_LoadTexture(renderer, "textures/options_1.png");
	textures[6] = IMG_LoadTexture(renderer, "textures/options_2.png");
	textures[7] = IMG_LoadTexture(renderer, "textures/options_3.png");
	textures[8] = IMG_LoadTexture(renderer, "textures/options_4.png");
	textures[9] = IMG_LoadTexture(renderer, "textures/resolution_default.png");
	textures[10] = IMG_LoadTexture(renderer, "textures/resolution_1.png");
	textures[11] = IMG_LoadTexture(renderer, "textures/resolution_2.png");
	textures[12] = IMG_LoadTexture(renderer, "textures/resolution_3.png");
	textures[13] = IMG_LoadTexture(renderer, "textures/resolution_4.png");
	textures[14] = IMG_LoadTexture(renderer, "textures/resolution_5.png");
	textures[15] = IMG_LoadTexture(renderer, "textures/resolution_6.png");
	textures[16] = IMG_LoadTexture(renderer, "textures/resolution_7.png");
	textures[17] = IMG_LoadTexture(renderer, "textures/resolution_8.png");
	textures[18] = IMG_LoadTexture(renderer, "textures/ingame_default.png");
	textures[19] = IMG_LoadTexture(renderer, "textures/ingame_1.png");
	textures[20] = IMG_LoadTexture(renderer, "textures/ingame_2.png");
	textures[21] = IMG_LoadTexture(renderer, "textures/ingame_3.png");
	textures[22] = IMG_LoadTexture(renderer, "textures/ingame_4.png");
	textures[23] = IMG_LoadTexture(renderer, "textures/X.png");
	textures[24] = IMG_LoadTexture(renderer, "textures/calling.png");
	textures[25] = IMG_LoadTexture(renderer, "textures/audience.png");
	textures[26] = IMG_LoadTexture(renderer, "textures/background.png");
	textures[27] = IMG_LoadTexture(renderer, "textures/howtoplay_default.png");
	textures[28] = IMG_LoadTexture(renderer, "textures/howtoplay_1.png");
	textures[29] = IMG_LoadTexture(renderer, "textures/ingame_correct1.png");
	textures[30] = IMG_LoadTexture(renderer, "textures/ingame_correct2.png");
	textures[31] = IMG_LoadTexture(renderer, "textures/ingame_correct3.png");
	textures[32] = IMG_LoadTexture(renderer, "textures/ingame_correct4.png");
	textures[33] = IMG_LoadTexture(renderer, "textures/options2_default.png");
	textures[34] = IMG_LoadTexture(renderer, "textures/options2_1.png");
	textures[35] = IMG_LoadTexture(renderer, "textures/options2_2.png");
	textures[36] = IMG_LoadTexture(renderer, "textures/options2_3.png");
	textures[37] = IMG_LoadTexture(renderer, "textures/options2_4.png");

	fiftyfifty = Mix_LoadWAV("audio/50_50.mp3");
	audience = Mix_LoadWAV("audio/audience.mp3");
	phone = Mix_LoadWAV("audio/phone.mp3");
	lesplay = Mix_LoadWAV("audio/lets_play.mp3");
	ga = Mix_LoadWAV("audio/correct_answer.mp3");
	ba = Mix_LoadWAV("audio/wrong_answer.mp3");
	fa = Mix_LoadWAV("audio/final_answer.mp3");
	firstfive = Mix_LoadMUS("audio/first_five_questions.mp3");
	secondfive = Mix_LoadMUS("audio/second_five_questions.mp3");
	thirdfive = Mix_LoadMUS("audio/third_five_questions.mp3");

	for(int i = 0; i < 14; ++i)
	{
		done_questions_arr[i].x = (int)(1323 / szorzo1);
		done_questions_arr[i].y = (int)(528 / szorzo2 - (i * (29 / szorzo2)));
		done_questions_arr[i].w = (int)(5 / szorzo1);
		done_questions_arr[i].h = (int)(5 / szorzo2);
	}
}

SDL_Rect Megjelenites::rectangle(int x, int y, int w, int h)
{
	SDL_Rect tmp;
	tmp.x = x;
	tmp.y = y;
	tmp.w = w;
	tmp.h = h;
	return tmp;
}

int Megjelenites::getQN()
{
	return QN;
}

SDL_Event* Megjelenites::addressOfEvent()
{
	return &event;
}

Uint32 Megjelenites::typeOfEvent()
{
	return event.type;
}

SDL_Renderer* Megjelenites::addressOfRenderer()
{
	return renderer;
}

int Megjelenites::strlen_UTF8(const char* s)
{
	int count = 0;
	while(*s)
	{
		count += (*s++ & 0xC0) != 0x80;
	}
	return count;
}

SDL_Texture* Megjelenites::texturefromstring(string line, SDL_Color color, char poz)
{
	TTF_Font* font;
	SDL_Surface* textSurface;

	if(strlen_UTF8(line.c_str()) >= 45 && poz == 'Q')
	{
		font = TTF_OpenFont("text/FontsFree-Net-arial-bold.ttf", 30);
	}
	else if(strlen_UTF8(line.c_str()) >= 25 && poz != 'Q' && poz != 'n')
	{
		font = TTF_OpenFont("text/FontsFree-Net-arial-bold.ttf", 22);
	}
	else if(strlen_UTF8(line.c_str()) >= 16 && poz != 'Q' && poz != 'n')
	{
		font = TTF_OpenFont("text/FontsFree-Net-arial-bold.ttf", 35);
	}
	else if(poz == 'n')
	{
		font = TTF_OpenFont("text/FontsFree-Net-arial-bold.ttf", 150);
	}
	else
	{
		font = TTF_OpenFont("text/FontsFree-Net-arial-bold.ttf", 45);
	}

	textSurface = TTF_RenderUTF8_Blended(font, line.c_str(), color);
	SDL_Texture* tmp = SDL_CreateTextureFromSurface(renderer, textSurface);
	
	switch(poz)
	{
		case 'Q':
			q1.x = (int)((389 + (1141.0 - textSurface->w) / 2) / szorzo1);
			q1.y = (int)((656 + (104.0 - textSurface->h) / 2) / szorzo2);
			q1.w = (int)(textSurface->w / szorzo1);
			q1.h = (int)(textSurface->h / szorzo2);
			break;
		case 'A':
			a1.x = (int)((389 + (489.0 - textSurface->w) / 2) / szorzo1);
			a1.y = (int)((799 + (104.0 - textSurface->h) / 2) / szorzo2);
			a1.w = (int)(textSurface->w / szorzo1);
			a1.h = (int)(textSurface->h / szorzo2);
			break;
		case 'B':
			a2.x = (int)((1041 + (489.0 - textSurface->w) / 2) / szorzo1);
			a2.y = (int)((799 + (104.0 - textSurface->h) / 2) / szorzo2);
			a2.w = (int)(textSurface->w / szorzo1);
			a2.h = (int)(textSurface->h / szorzo2);
			break;
		case 'C':
			a3.x = (int)((389 + (489.0 - textSurface->w) / 2) / szorzo1);
			a3.y = (int)((942 + (104.0 - textSurface->h) / 2) / szorzo2);
			a3.w = (int)(textSurface->w / szorzo1);
			a3.h = (int)(textSurface->h / szorzo2);
			break;
		case 'D':
			a4.x = (int)((1041 + (489.0 - textSurface->w) / 2) / szorzo1);
			a4.y = (int)((942 + (104.0 - textSurface->h) / 2) / szorzo2);
			a4.w = (int)(textSurface->w / szorzo1);
			a4.h = (int)(textSurface->h / szorzo2);
			break;
		case 'a':
			a.x = (int)((377 + (50.0 - textSurface->w) / 2) / szorzo1);
			a.y = (int)((809 + (84.0 - textSurface->h) / 2) / szorzo2);
			a.w = (int)(textSurface->w / szorzo1);
			a.h = (int)(textSurface->h / szorzo2);
			break;
		case 'b':
			b.x = (int)((1029 + (50.0 - textSurface->w) / 2) / szorzo1);
			b.y = (int)((809 + (84.0 - textSurface->h) / 2) / szorzo2);
			b.w = (int)(textSurface->w / szorzo1);
			b.h = (int)(textSurface->h / szorzo2);
			break;
		case 'c':
			c.x = (int)((377 + (50.0 - textSurface->w) / 2) / szorzo1);
			c.y = (int)((952 + (84.0 - textSurface->h) / 2) / szorzo2);
			c.w = (int)(textSurface->w / szorzo1);
			c.h = (int)(textSurface->h / szorzo2);
			break;
		case 'd':
			d.x = (int)((1029 + (50.0 - textSurface->w) / 2) / szorzo1);
			d.y = (int)((952 + (84.0 - textSurface->h) / 2) / szorzo2);
			d.w = (int)(textSurface->w / szorzo1);
			d.h = (int)(textSurface->h / szorzo2);
			break;
		case 'n':
			ny_poz.x = (int)((394 + (1132.0 - textSurface->w) / 2) / szorzo1);
			ny_poz.y = (int)((475 + (130.0 - textSurface->h) / 2) / szorzo2);
			ny_poz.w = (int)(textSurface->w / szorzo1);
			ny_poz.h = (int)(textSurface->h / szorzo2);
			break;
		case 'u':
			perc1_poz.x = (int)((545 + (28.0 - textSurface->w) / 2) / szorzo1);
			perc1_poz.y = (int)((53 + (32.0 - textSurface->h) / 2) / szorzo2);
			perc1_poz.w = (int)(textSurface->w / szorzo1);
			perc1_poz.h = (int)(textSurface->h / szorzo2);
		case 'i':
			perc2_poz.x = (int)((705 + (28.0 - textSurface->w) / 2) / szorzo1);
			perc2_poz.y = (int)((53 + (32.0 - textSurface->h) / 2) / szorzo2);
			perc2_poz.w = (int)(textSurface->w / szorzo1);
			perc2_poz.h = (int)(textSurface->h / szorzo2);
		case 'o':
			perc3_poz.x = (int)((871 + (28.0 - textSurface->w) / 2) / szorzo1);
			perc3_poz.y = (int)((53 + (32.0 - textSurface->h) / 2) / szorzo2);
			perc3_poz.w = (int)(textSurface->w / szorzo1);
			perc3_poz.h = (int)(textSurface->h / szorzo2);
		case 'p':
			perc4_poz.x = (int)((1029 + (28.0 - textSurface->w) / 2) / szorzo1);
			perc4_poz.y = (int)((53 + (32.0 - textSurface->h) / 2) / szorzo2);
			perc4_poz.w = (int)(textSurface->w / szorzo1);
			perc4_poz.h = (int)(textSurface->h / szorzo2);
	}

	SDL_FreeSurface(textSurface);
	return tmp;
}

void Megjelenites::changeForResolution()
{
	r1 = rectangle((int)(353 / szorzo1), (int)(842 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));
	r2 = rectangle((int)(1005 / szorzo1), (int)(842 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));
	r3 = rectangle((int)(353 / szorzo1), (int)(985 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));
	r4 = rectangle((int)(1005 / szorzo1), (int)(985 / szorzo2), (int)(20 / szorzo1), (int)(20 / szorzo2));

	x1 = rectangle((int)(1257 / szorzo1), (int)(50 / szorzo2), (int)(87 / szorzo1), (int)(44 / szorzo2));
	x2 = rectangle((int)(1343 / szorzo1), (int)(50 / szorzo2), (int)(87 / szorzo1), (int)(44 / szorzo2));
	x3 = rectangle((int)(1430 / szorzo1), (int)(50 / szorzo2), (int)(87 / szorzo1), (int)(44 / szorzo2));

	calldest = rectangle((int)(383 / szorzo1), (int)(475 / szorzo2), (int)(819 / szorzo1), (int)(110 / szorzo2));
	audiencedest = rectangle((int)(383 / szorzo1), (int)(28 / szorzo2), (int)(819 / szorzo1), (int)(436 / szorzo2));

	cq_designate = rectangle((int)(1246 / szorzo1), (int)(518 / szorzo2), (int)(278 / szorzo1), (int)(21 / szorzo2));

	column_a.x = (int)(536 / szorzo1); column_a.w = (int)(45 / szorzo1);
	column_b.x = (int)(696 / szorzo1); column_b.w = (int)(45 / szorzo1);
	column_c.x = (int)(856 / szorzo1); column_c.w = (int)(45 / szorzo1);
	column_d.x = (int)(1016 / szorzo1);  column_d.w = (int)(45 / szorzo1);
	if(SCREEN_HEIGHT == 720) { column_a.y = (int)(386 / szorzo2 - 1); column_b.y = (int)(386 / szorzo2 - 1); column_c.y = (int)(386 / szorzo2 - 1); column_d.y = (int)(386 / szorzo2 - 1); }
	else { column_a.y = (int)(386 / szorzo2); column_b.y = (int)(386 / szorzo2); column_c.y = (int)(386 / szorzo2); column_d.y = (int)(386 / szorzo2); }

	SDL_DestroyTexture(atext);
	SDL_DestroyTexture(btext);
	SDL_DestroyTexture(ctext);
	SDL_DestroyTexture(dtext);

	atext = texturefromstring("A:", orange, 'a');
	btext = texturefromstring("B:", orange, 'b');
	ctext = texturefromstring("C:", orange, 'c');
	dtext = texturefromstring("D:", orange, 'd');

	for(int i = 0; i < 14; ++i)
	{
		done_questions_arr[i].x = (int)(1323 / szorzo1);
		done_questions_arr[i].y = (int)(528 / szorzo2 - (i * (29 / szorzo2)));
		done_questions_arr[i].w = (int)(5 / szorzo1);
		done_questions_arr[i].h = (int)(5 / szorzo2);
	}
}

void Megjelenites::setHiders(int poz, int previous)
{
	if(previous == -1)
	{
		switch(poz)
		{
			case 0:
				hider_dest1 = rectangle((int)(430 / szorzo1), (int)(805 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
			case 1:
				hider_dest1 = rectangle((int)(1080 / szorzo1), (int)(805 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
			case 2:
				hider_dest1 = rectangle((int)(430 / szorzo1), (int)(945 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
			case 3:
				hider_dest1 = rectangle((int)(1080 / szorzo1), (int)(945 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
		}
	}
	else
	{
		switch(poz)
		{
			case 0:
				hider_dest2 = rectangle((int)(430 / szorzo1), (int)(805 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
			case 1:
				hider_dest2 = rectangle((int)(1080 / szorzo1), (int)(805 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
			case 2:
				hider_dest2 = rectangle((int)(430 / szorzo1), (int)(945 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
			case 3:
				hider_dest2 = rectangle((int)(1080 / szorzo1), (int)(945 / szorzo2), (int)(445 / szorzo1), (int)(90 / szorzo2));
				break;
		}
	}
}

void Megjelenites::setCallParameters(Jatek& game)
{
	if(game.getTelefonos().getLetter() == 'A') { calldestletter.w = a.w; calldestletter.h = a.h; }
	else if(game.getTelefonos().getLetter() == 'B') { calldestletter.w = b.w; calldestletter.h = b.h; }
	else if(game.getTelefonos().getLetter() == 'C') { calldestletter.w = c.w; calldestletter.h = c.h; }
	else { calldestletter.w = d.w; calldestletter.h = d.h; }
	calldestletter.x = (int)(1136 / szorzo1 + (54.0 / szorzo1 - calldestletter.w) / 2);
	calldestletter.y = (int)(480 / szorzo2 + (100.0 / szorzo2 - calldestletter.h) / 2);
	hider_dest3 = rectangle((int)(1174 / szorzo1), (int)(517 / szorzo2), (int)(14 / szorzo1), (int)(33 / szorzo2));
}

void Megjelenites::setAudienceParameters(Jatek& game)
{
	column_a.h = (int)(game.getKozonseg().getSzorzo(0) * (280 / szorzo2) * -1);
	column_b.h = (int)(game.getKozonseg().getSzorzo(1) * (280 / szorzo2) * -1);
	column_c.h = (int)(game.getKozonseg().getSzorzo(2) * (280 / szorzo2) * -1);
	column_d.h = (int)(game.getKozonseg().getSzorzo(3) * (280 / szorzo2) * -1);
	perc1 = texturefromstring(std::to_string((int)round(game.getKozonseg().getSzorzo(0) * 100)) + "%", white, 'u');
	perc2 = texturefromstring(std::to_string((int)round(game.getKozonseg().getSzorzo(1) * 100)) + "%", white, 'i');
	perc3 = texturefromstring(std::to_string((int)round(game.getKozonseg().getSzorzo(2) * 100)) + "%", white, 'o');
	perc4 = texturefromstring(std::to_string((int)round(game.getKozonseg().getSzorzo(3) * 100)) + "%", white, 'p');
}

void Megjelenites::render_cq_designate()
{
	if(QN > 0 && QN != previous)
	{
		cq_designate.y = (int)(518 / szorzo2 - (QN * (29 / szorzo2)));
		previous = QN;
	}

	SDL_SetRenderDrawColor(renderer, 226, 63, 4, 191);
	SDL_RenderFillRect(renderer, &cq_designate);
}

void Megjelenites::render_done_questions()
{
	if(QN > 0)
	{
		switch(QN)
		{
			case 1:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 2:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 3:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 4:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 5:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 6:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 7:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 8:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 9:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[8], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 10:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[8], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[9], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 11:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[8], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[9], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[10], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 12:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[8], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[9], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[10], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[11], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 13:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[8], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[9], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[10], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[11], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[12], 45.0, NULL, SDL_FLIP_NONE);
				break;
			case 14:
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[0], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[1], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[2], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[3], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[4], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[5], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[6], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[7], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[8], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[9], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[10], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[11], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[12], 45.0, NULL, SDL_FLIP_NONE);
				SDL_RenderCopyEx(renderer, done_questions, NULL, &done_questions_arr[13], 45.0, NULL, SDL_FLIP_NONE);
				break;
		}
	}
}
