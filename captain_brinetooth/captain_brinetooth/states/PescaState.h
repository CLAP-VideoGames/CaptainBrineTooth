#pragma once
#include "GameState.h"
#include "box2d.h"
class DamageCollisionCallbacks;
class Entity;

class PescaState :public GameState
{
public:
	PescaState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* p) : GameState(a, mundo, snd) { playerRef = p; };
	virtual ~PescaState();
	virtual void init();
	virtual void update();

	void createPesca(const Config& entityConfig);
	void createRandomReward(const Config& entityConfig);
	void crearBasura();
	App* getApp() { return app; }
protected:
	Entity* playerRef;
	SDL_Rect cam;
	int entitiesPerLine, totalBasura;
	int rows_ = 3;
	std::vector<int> rowHeights;
	float screen_width;
	float screen_heigth;
	int w_reward;
	int h_reward;
	float main_zoom;
	int WeaponsNumber;
	//DamageCollisionCallbacks collisionListener;
	
};

