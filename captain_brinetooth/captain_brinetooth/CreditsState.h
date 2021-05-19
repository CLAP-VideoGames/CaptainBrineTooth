#pragma once
#include "../captain_brinetooth/states/GameState.h"

class DamageCollisionCallbacks;
class Entity;

class CreditsState :public GameState
{
public:
	CreditsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* p, std::shared_ptr <b2World> maux) : GameState(a, mundo, snd) {};
	virtual ~CreditsState();
	virtual void init();
	virtual void update();

	
	App* getApp() { return app; }
protected:
	

};



