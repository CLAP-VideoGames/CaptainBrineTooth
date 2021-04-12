#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class TestZoneState :public GameState
{
public:
	TestZoneState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual void init();

	void createPlayer(const Config& playerConfig);
	void createBoxFloor(const Config& entityConfig);
	void createEnemy1(const Config& enemy1Config);
	void createElfShark(const Config& entityConfig);
protected:
	SDL_Rect cam;
};

