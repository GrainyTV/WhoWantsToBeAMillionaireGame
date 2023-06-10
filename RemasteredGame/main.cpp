#include "Game.hpp"

int main(int argc, char* args[])
{
	Game g_Game = Game::Instance();
	g_Game.Run();

	return 0;
}