#include "Game.hpp"

using std::exception;

static const int GREEN = 32;
static const int RED = 31;
static const int ORANGE = 33;

static void ColoredPrint(const char* string, const int& color)
{
	printf("\x1B[%dm%s\033[0m\n", color, string);
}

int main(int argc, char* args[])
{
	try
	{
		Game* const g_Game = Game::Instance();
		(*g_Game).FillFileNames();
		(*g_Game).StoreData();
		//(*g_Game).Testing();
		(*g_Game).Launch();	
	}
	catch(const exception& e)
	{
		ColoredPrint(e.what(), ORANGE);
		ColoredPrint("Program exited with code: 1", RED);
		return EXIT_FAILURE;
	}

	ColoredPrint("Program exited with code: 0", GREEN);
	return EXIT_SUCCESS;
}