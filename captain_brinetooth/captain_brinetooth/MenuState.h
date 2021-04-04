#pragma once
#include "GameState.h"
#include "components/SoundManager.h"
#include "box2d.h"

class Entity;

class MenuState : public GameState
{
public:

	MenuState(Game* game,b2World* mundo);
	void addStateEntityMenu();
	static void changeToGame(Game* game);
	static void PlayGaviota();
	void update();

private:
	SDL_Rect cam;
	
};

