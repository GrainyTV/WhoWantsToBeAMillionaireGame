#include "Event.hpp"
#include "Game.hpp"

/**
 * 
 * Constructor with a copy of our scene object.
 * @param scene : a pointer to our previously created scene
 * 
 */
Event::Event(Scene* scene)
{
	scenePtr = scene;

	FillMouseHitboxes();

	// User-invokable events and their method calls
	eventCalls[SDL_QUIT] = [] () { Game::Terminate(); };
	eventCalls[SDL_USEREVENT] = [&, this] () { (*scenePtr).Redraw(); };
	eventCalls[SDL_MOUSEBUTTONDOWN] = [&, this] () { (*this).MouseClick(); };
	eventCalls[SDL_MOUSEMOTION] = [&, this] () { (*this).MouseMotion(gameEvent); };
}

/**
 * 
 * Getter function for the game event object.
 * @return : the address of the game event
 * 
 */
SDL_Event* Event::_IncomingGameEvent()
{
	return &gameEvent;
}

/**
 * 
 * Validator for the incoming events. It's considered valid if we had registered it.
 * @return : either true or false
 * 
 */
bool Event::IsValidEvent() const
{
	if(eventCalls.contains(gameEvent.type))
	{
		return true;
	}

	return false;
}

/**
 * 
 * Call the appropriate method based on the event.
 * 
 */
void Event::ExecuteHandler()
{	
	eventCalls[gameEvent.type]();
}

/**
 * 
 * Handling mouse click events.
 * 
 */
void Event::MouseClick()
{
	GameState currentState = (*scenePtr)._State();
	int textureID = (*scenePtr)._TextureId();

	// 0 Means cursor is not on clickable surface
	if(textureID != 0)
	{
		// Subtract 1 from ID,
		// Indexing starts at 0
		buttonHitboxesClicked[currentState][textureID - 1]();
		(*scenePtr).Invalidate();
	}
}

/**
 * 
 * Handling mouse movement events.
 * 
 */
void Event::MouseMotion(SDL_Event args)
{
	int x = args.motion.x;
	int y = args.motion.y;

	GameState currentState = (*scenePtr)._State();
	SDL_Rect mousePos = SDL_Rect(x, y, 1, 1);
	int previousID = (*scenePtr)._TextureId();
	int textureID = 0;

	// Chechking wether mouse is on a clickable surface / button
	for(int i = 0; i < buttonHitboxes[currentState].size(); ++i)
	{
		if(Hit(buttonHitboxes[currentState][i], mousePos))
		{
			textureID = i + 1;
			break;
		}
	}

	// Checking wether mouse moved outside of the previous hitbox
	if(textureID != previousID)
	{
		(*scenePtr)._TextureId(textureID);
		(*scenePtr).Invalidate();
	}
}

/**
 * 
 * Fills our two hashtables. (Hover and Click)
 * The first one is the position of our clickable objects. (SDL_Rect)
 * The second one is the methods called after a click on them. (function<void()>)
 * 
 */
void Event::FillMouseHitboxes()
{
	// Mainmenu (hover && clicked)
	deque<SDL_Rect> mainMenuHover = { (SDL_Rect) { 665, 650, 595, 110 }, (SDL_Rect) { 665, 795, 595, 110 }, (SDL_Rect) { 665, 940, 595, 110 } };
	buttonHitboxes[GameState::MainMenu] = mainMenuHover;
	
	deque<function<void()>> mainMenuClicked = 
	{ 
		[&, this] () { (*Game::Instance()).GenerateQuestions(); }, //Game::GenerateQuestions(); (*scenePtr)._State(GameState::InGame); },
		[&, this] () { (*scenePtr)._State(GameState::Options); },
		[] () { Game::Terminate(); }
	};
	buttonHitboxesClicked[GameState::MainMenu] = mainMenuClicked;

	// Ingame (hover && clicked)
	deque<SDL_Rect> inGameHover = { (SDL_Rect) { 336, 796, 592, 110 }, (SDL_Rect) { 990, 796, 592, 110 }, (SDL_Rect) { 336, 940, 592, 110 }, (SDL_Rect) { 990, 940, 592, 110 } };
	buttonHitboxes[GameState::InGame] = inGameHover;

	// Options (hover && clicked)
	/*
	.
	.
	.
	*/
}


/**
 * 
 * Checks wether the two given rectangles intersect each other.
 * @param r1 : the first rectangle
 * @param r2 : the second rectangle (this will usually be a 1px square of the cursor)
 * return : either true or false
 * 
 */
bool Event::Hit(const SDL_Rect& r1, const SDL_Rect& r2)
{
	auto hit = SDL_HasIntersection(&r1, &r2);
	return (hit == SDL_TRUE) ? true : false;
}