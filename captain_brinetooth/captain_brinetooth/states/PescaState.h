#pragma once
#include "GameState.h"
#include "box2d.h"
class DamageCollisionCallbacks;
class Entity;

class PescaState :public GameState
{
public:
	PescaState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* p, std::shared_ptr <b2World> maux); //: GameState(a, mundo, snd) { playerRef = p;  playWorld = maux; p->getComponent<PlayerController>()->setMoveRight(false); p->getComponent<PlayerController>()->setMoveLeft(false);
	;
	virtual ~PescaState();
	virtual void init();
	virtual void update();

	void createPesca(const Config& entityConfig);
	void createRandomReward(const Config& entityConfig);
	void crearBasura();
	bool checkBasura(int x, int y);
	App* getApp() { return app; }
protected:
	Entity* playerRef;
	SDL_Rect cam;
	std::shared_ptr<b2World> playWorld;
	int entitiesPerLine, totalBasura;
	int rows_ = 3;
	std::vector<int> rowHeights;
	float screen_width;
	float screen_heigth;
	int w_reward;
	int h_reward;
	float main_zoom;
	int WeaponsNumber;
	bool space_pressed_;
	Entity* s;
	Entity*	m3;
	std::vector<Entity*> basuras;
	//DamageCollisionCallbacks collisionListener;
	
};

