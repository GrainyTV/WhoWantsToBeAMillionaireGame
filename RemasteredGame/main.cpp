#include "Game.hpp"

using std::exception;

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
		(*g_Game).Run();
	}
	catch(const exception& e)
	{
		// Color Value 33 = Yellow
		ColoredPrint(e.what(), 33);

		// Color Value 31 = Red
		ColoredPrint("Program exited with code: 1", 31);
		return EXIT_FAILURE;
	}

	// Color Value 32 = Green
	ColoredPrint("Program exited with code: 0", 32);
	return EXIT_SUCCESS;
}