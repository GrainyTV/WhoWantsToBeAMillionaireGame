#include "Game.hpp"

unique_ptr<Game> Game::instance = nullptr;

/**
 * 
 * Game object constructor that gets called once when the application starts.
 * 
 */
Game::Game()
{
	srand((unsigned int)(time(NULL)));
	setlocale(LC_ALL, "hu_HU.utf8");
}

void Game::Run()
{
	for(auto keyValuePair : fileNames)
	{
		auto key = keyValuePair.first;
		Data::LoadContent(fileNames.at(key));
	}
}

/**
 * 
 * Initializes the input files for our registered categories. We can add new ones by extending this method.
 * 
 */
void Game::FillFileNames()
{
	fileNames[Category::Art] = "art.txt";
	fileNames[Category::Astronomy] = "astronomy.txt";
	fileNames[Category::Food] = "food.txt";
	fileNames[Category::Geography] = "geography.txt";
	fileNames[Category::History] = "history.txt";
	fileNames[Category::Holidays] = "holidays.txt";
	fileNames[Category::Literature] = "literature.txt";
	fileNames[Category::Music] = "music.txt";
	fileNames[Category::Nature] = "nature.txt";
	fileNames[Category::Other] = "other.txt";
	fileNames[Category::Proverbs] = "proverbs.txt";
	fileNames[Category::TelevisionSeries] = "series.txt";
	fileNames[Category::Sports] = "sports.txt";
	fileNames[Category::Tabloid] = "tabloid.txt";
	fileNames[Category::Theatre] = "theatre.txt";
	fileNames[Category::Transport] = "transport.txt";
}

/**
 * 
 * Returns our singleton game object when called. If it hadn't been called before, it gets initialized.
 * 
 */
Game* Game::Instance()
{
	if(instance == nullptr)
	{
		instance = unique_ptr<Game>(new Game());
	}

	return instance.get();
}
