#include "Game.hpp"
#include "CategoryChance.hpp"

unique_ptr<Game> Game::instance = nullptr;

bool Game::running = true;

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

/**
 * 
 * Stores all the input data if the loading process was successful.
 * All the data is sorted by category then difficulty for ease of use.
 * 
 * [Category] -> [Easy]   -> [Data0, Data1, Data2, ...]
 *            -> [Medium] -> [Data0, Data1, Data2, ...]
 *            -> [Hard]   -> [Data0, Data1, Data2, ...]
 * 
 */
void Game::StoreData()
{
	for(auto keyValuePair : fileNames)
	{
		// Category(first) - "example.txt"(second)
		auto key = keyValuePair.first;
		
		// Initializing empty containers for the data
		deque<Data> defaultList;
		unordered_map<Difficulty, deque<Data>> defaultMap = { { Easy, defaultList }, { Medium, defaultList }, { Hard, defaultList } };
		gameData[key] = defaultMap;

		// Iterating over every element,
		// and pushing them to their appropriate container
		for(auto data : Data::LoadContent(fileNames.at(key)))
		{
			switch(data._Difficulty())
			{
			case Easy:
				gameData[key][Easy].push_back(data);
				break;
			case Medium:
				gameData[key][Medium].push_back(data);
				break;
			case Hard:
				gameData[key][Hard].push_back(data);
				break;
			}
		}
	}
}

/**
 * 
 * Initializes the input files for our registered categories. We can add new ones by extending this method.
 * 
 */
void Game::FillFileNames()
{
	path directory = "text";
	
	for(auto entry : directory_iterator(directory.c_str()))
	{
		// Query for the path of the file
		path fileWithDirectory = entry.path();

		// Create a new enum value from the name of the file without extension
		string newEnumValue = fileWithDirectory.stem().string();

		// Add entry to our enum object
		category.Add(newEnumValue);

		// Save .txt file in our hashmap
		fileNames[category[newEnumValue]] = fileWithDirectory.string();
	}
}

/**
 * 
 * Testing method to see, if our input files are loaded.
 * 
 */
void Game::Testing()
{
	for(auto values : gameData[category["theatre"]][Easy])
	{
		values.Print();
	}

	printf("\n");

	for(auto values : gameData[category["tabloid"]][Medium])
	{
		values.Print();
	}

	printf("\n");

	for(auto values : gameData[category["other"]][Hard])
	{
		values.Print();
	}

	printf("\n");

	for(auto enumKeyValue : category)
	{
		printf("%s - %lu\n", enumKeyValue.name.c_str(), enumKeyValue.value);
	}
}

/**
 * 
 * Creates a view for the game and launches it.
 * 
 */
void Game::Launch()
{
	frontEnd = unique_ptr<GameLoop>(new GameLoop());
	(*frontEnd.get()).EnterGameLoop();
}

/**
 * 
 * Creates 15 randomly chosen questions for a new game.
 * @return : the selected questions' container
 * 
 */
deque<Data> Game::GenerateNewGame()
{
	CategoryChance chances(category);
	deque<string> categories = chances.GenerateRandomizedCategories();
	Difficulty currentDiff = Difficulty::Easy;
	deque<Data> chosenQuestions;

	for(int i = 0; i < categories.size(); ++i)
	{
		deque<Data> options = gameData[category[categories[i]]][currentDiff];
		bool success = false;
		
		do
		{
			const unsigned int RANDOM = CategoryChance::Random(0, options.size() - 1);
			const Data QUESTION = options[RANDOM];
			const bool CONTAINS = find(chosenQuestions.begin(), chosenQuestions.end(), QUESTION) != chosenQuestions.end();

			if(CONTAINS == false)
			{
				chosenQuestions.push_back(QUESTION);
				success = true;
			}

		} while(success == false);

		if((i + 1) % 5 == 0)
		{
			currentDiff = static_cast<Difficulty>((i + 1) / 5);
		}
	}

	return chosenQuestions;
}

/**
 * 
 * Returns our singleton game object when called. If it hadn't been called before, it gets initialized.
 * @return : a pointer to our instance
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

/**
 * 
 * Indicator wether the game is still running.
 * @return : either true or false
 * 
 */
bool Game::IsRunning()
{
	return running;
}

/**
 * 
 * Function to terminate the application.
 * 
 */
void Game::Terminate()
{
	running = false;
}