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
	void crearBasura();
	App* getApp() { return app; }
protected:
	SDL_Rect cam;
	int entitiesPerLine, totalBasura;
	int rows_ = 3;
	std::vector<int> rowHeights;
	float screen_width;
	float screen_heigth;
	int w_reward;
	int h_reward;
	float main_zoom;
	//DamageCollisionCallbacks collisionListener;
	
};

