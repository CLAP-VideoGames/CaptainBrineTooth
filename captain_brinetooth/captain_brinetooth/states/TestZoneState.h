#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class TestZoneState :public GameState
{
public:
	TestZoneState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd) {};
	virtual void init();
	virtual void update();

	void createPlayer(const Config& playerConfig);
	void createBoxFloor(const Config& entityConfig);
	void createPompeyWorm(const Config& enemy1Config);
	void createElfShark(const Config& entityConfig);
	void createPesca(const Config& entityConfig);
	void createFlowerJellyHat(const Config& entityConfig);
	void createFringeHead(const Config& entityConfig);
	void createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType);
protected:
	SDL_Rect cam {};
};

