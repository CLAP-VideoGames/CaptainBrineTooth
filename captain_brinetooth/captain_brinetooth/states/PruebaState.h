#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class PruebaState :public GameState
{
public:

	PruebaState(App* a, b2World* mundo, SoundManager* snd);
	virtual void init();

};

