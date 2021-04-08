#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class TestZoneState :public GameState
{
public:

	TestZoneState(App* a, b2World* mundo, SoundManager* snd);
	virtual void init();

};

