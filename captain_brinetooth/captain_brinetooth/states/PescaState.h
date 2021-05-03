#pragma once
#include "GameState.h"
#include "box2d.h"
class DamageCollisionCallbacks;
class Entity;

class PescaState :public GameState
{
public:
	PescaState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd) {};
	virtual void init();
	virtual void update();

	void createPesca(const Config& entityConfig);
	void createRandomReward(const Config& entityConfig);
	App* getApp() { return app; }
protected:
	SDL_Rect cam;
	int entitiesPerLine;
	//DamageCollisionCallbacks collisionListener;
	
};

