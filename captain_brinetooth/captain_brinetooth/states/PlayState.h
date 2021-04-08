#pragma once
#include "GameState.h"

class Entity;

class PlayState : public GameState
{
public:
	PlayState(App* a, b2World* mundo, SoundManager* snd);
	virtual void init();
	void update();

private:
	SDL_Rect cam;

};
