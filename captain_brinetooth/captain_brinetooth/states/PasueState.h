#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class PauseState :public GameState
{
public:

	PauseState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual void init();

};