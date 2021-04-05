#pragma once
#include "GameState.h"
#include "box2d.h"

class Entity;

class MenuState : public GameState
{
public:

	MenuState(Game* a, b2World* mundo, SoundManager* snd);
	void addStateEntityMenu();
	static void changeToGame(Game* game, SoundManager* snd);
	static void salirMenu(Game* game, SoundManager* snd);
	void update();
	void setSoundController(SoundManager* snd) { soundController = snd; }

private:
	SDL_Rect cam;
	
};

