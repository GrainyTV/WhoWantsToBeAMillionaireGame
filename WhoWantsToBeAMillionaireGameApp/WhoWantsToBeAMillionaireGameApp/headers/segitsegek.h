#ifndef SEGITSEGEK_H
#define SEGITSEGEK_H
class Megjelenites;
class Jatek;

class Segitsegek
{
	bool isAlreadyUsed = false;
	int current_round;

	public:

	bool& wasItUsed();

	void setUsedTrue();

	void setCurrentRound(Megjelenites& display);

	int getCurrentRound();

	void resetHelp();
};

class Felezo : public Segitsegek
{
	int firstScreenHiderIdx = -1;
	int secondScreenHiderIdx = -1;

	public:

	int getFSHI();

	int getSSHI();

	void setFSHI(int idx);

	void setSSHI(int idx);

	void halving(Jatek& game, Adatok& data, Megjelenites& display, int previous = -1);
};

class Telefonos : public Segitsegek
{
	char sol_letter;

	public:

	void givehint(Jatek& game, Adatok& data, Megjelenites& display);

	char getLetter();
};

class Kozonseg : public Segitsegek
{
	double sz_A;
	double sz_B;
	double sz_C;
	double sz_D;

	public:

	void createchart(Jatek& game, Adatok& data, Megjelenites& display);

	double getSzorzo(int idx);
};


#endif
