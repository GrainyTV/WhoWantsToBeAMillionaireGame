#include "jatekciklus.h"

Jatek::Jatek(Adatok& data)
{
	for(int i = 0; i < catLength; ++i)
	{	
		questions[i] = data.length("text/" + input[i]);
		questions[catLength] += data.length("text/" + input[i]);
	}

	arr = data.read(input, questions, questions[catLength], categories, catLength);
}

Adatok* Jatek::getloc()
{
	return arr;
}

int Jatek::getlen(int idx)
{
	return questions[idx];
}

bool& Jatek::isrunning()
{
	return running;
}

string Jatek::catChoosing(int idx)
{
	return categories[idx];
}

int Jatek::getCatLength()
{
	return catLength;
}

int Jatek::getCatIdx(string tema)
{
	int tmp = 0;

	for(int i = 0; i < catLength; ++i)
	{
		if(categories[i] == tema)
		{
			tmp = i;
			break;
		}
	}

	return tmp;
}

Kerdesek* Jatek::getKerdesek()
{
	return previous;
}

Valaszok* Jatek::getValaszok()
{
	return choosable;
}

Felezo& Jatek::getFelezo()
{
	return f1;
}

Telefonos& Jatek::getTelefonos()
{
	return t1;
}

Kozonseg& Jatek::getKozonseg()
{
	return k1;
}