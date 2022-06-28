#ifndef JATEKCIKLUS_H
#define JATEKCIKLUS_H
#include "adatok.h"
#include "kerdesekvalaszok.h"
#include "segitsegek.h"

class Jatek
{
	Adatok* arr;
	Kerdesek previous[15];
	Valaszok choosable[4];
	Felezo f1;
	Telefonos t1;
	Kozonseg k1;
	
	bool running = true;
	static const int catLength = 16;
	int questions[catLength + 1] = { 0 };
	string input[catLength] = { "history.txt", "geography.txt", "tabloid.txt", "nature.txt", "music.txt", "sports.txt", "theatre.txt",
	"art.txt", "literature.txt", "food.txt", "holidays.txt", "astronomy.txt", "series.txt", "transport.txt", "proverbs.txt", "other.txt" };
	string categories[catLength] = { "History", "Geography", "Tabloid", "Nature", "Music", "Sports", "Theatre", "Art", "Literature",
	"Food", "Holidays", "Astronomy", "Series", "Transport", "Proverbs", "Other" };

	public:

	Jatek(Adatok& data);

	Adatok* getloc();

	int getlen(int idx);

	bool& isrunning();

	void terminateRunning();

	string catChoosing(int idx);

	int getCatLength();

	int getCatIdx(string tema);

	Kerdesek* getKerdesek();

	Valaszok* getValaszok();

	Felezo& getFelezo();

	Telefonos& getTelefonos();

	Kozonseg& getKozonseg();
};

#endif 

