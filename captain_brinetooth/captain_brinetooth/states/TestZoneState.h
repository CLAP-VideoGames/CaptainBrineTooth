#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class TestZoneState :public GameState
{
public:

	TestZoneState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual void init();

};

