#ifndef MEGJELENITES_H
#define MEGJELENITES_H
#include <iostream>
#include "jatekciklus.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

using std::string;

class Megjelenites
{
	int SCREEN_WIDTH, SCREEN_HEIGHT, resolution, valto, x, y, QN, starttime, previous, serial_nb;
	bool fullscreen, allow_control, allow_control2, therewasananswer, playedsound, didtheplayerlose, audiencedelaywasused, phonedelaywasused, halveningdelaywasused;
	double szorzo1, szorzo2;
	string current_screen;
	string prices[16] = { "0", "5.000", "10.000", "25.000", "50.000", "100.000", "200.000", "300.000", "500.000", "800.000", "1.500.000", "3.000.000", "5.000.000", "10.000.000", "20.000.000" , "40.000.000" };
	
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;
	TTF_Font* font;
	SDL_Color white, orange;
	SDL_Texture* textures[39];
	SDL_Texture* current_question, * curr_a1, * curr_a2, * curr_a3, * curr_a4;
	SDL_Texture* rombusz, * atext, * btext, * ctext, * dtext;
	SDL_Texture* nyeremeny, * perc1, * perc2, * perc3, * perc4;
	SDL_Texture* done_questions;
	SDL_Rect perc1_poz, perc2_poz, perc3_poz, perc4_poz;
	SDL_Rect dest, hider_dest1, hider_dest2, hider_dest3, calldest, calldestletter, cdr, audiencedest;
	SDL_Rect q1, a1, a2, a3, a4, ny_poz;
	SDL_Rect a, b, c, d;
	SDL_Rect column_a, column_b, column_c, column_d;
	SDL_Rect r1, r2, r3, r4;
	SDL_Rect x1, x2, x3;
	SDL_Rect cq_designate;
	SDL_Rect done_questions_arr[14];
	Mix_Music* firstfive, * secondfive, * thirdfive;
	Mix_Chunk* fiftyfifty, * audience, * phone, * pause, * lesplay, * ga, * ba, * fa;

	public:

	Megjelenites();

	void setserial_nb(int number);

	int getserial_nb();

	SDL_Rect rectangle(int x, int y, int w, int h);

	int strlen_UTF8(const char* s);

	SDL_Texture* texturefromstring(string line, SDL_Color color, char poz);

	void new_game(Jatek& game, Adatok& data);

	int getQN();

	void mouseclick(Jatek& game, Adatok& data, Megjelenites& display);

	void setHiders(int poz, int previous);

	int question_selector(Jatek& game, Adatok& data);

	void nextquestion(Jatek& game, Adatok& data);

	void mousepos(Jatek& game);

	void screen_renderer(Jatek& game, Adatok& data);

	void screen_renderer_ING(Jatek& game, Adatok& data);

	void render_background(Jatek& game, Adatok& data);

	void render_question();

	void render_answerA();

	void render_answerB();

	void render_answerC();

	void render_answerD();

	void renderX(Jatek& game);

	void renderSegitsegek(Jatek& game);

	SDL_Event* addressOfEvent();

	Uint32 typeOfEvent();

	SDL_Renderer* addressOfRenderer();

	void freeResources(Jatek& game);

	void changeForResolution();

	void setCallParameters(Jatek& game);

	void setAudienceParameters(Jatek& game);

	void render_cq_designate();

	void render_done_questions();
};

#endif
