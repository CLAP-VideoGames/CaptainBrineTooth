#pragma once
#include "GameState.h"
class Entity;

class PlayState : public GameState
{
public:
	PlayState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual ~PlayState();

	virtual void init();
	virtual void update();

	void createLevel0();
	void createPlayer(const Config& playerConfig);
	void createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType);
	void createBoxTest(const Config& entityConfig);
	void createElfShark(const Config& entityConfig);
	void createFlowerJellyHat(const Config& entityConfig);


private:
	SDL_Rect cam;
	MapProcedural* map;
	Vector2D camLimits;
};
