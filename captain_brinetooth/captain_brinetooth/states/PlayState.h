#pragma once
#include "GameState.h"
class Entity;
struct infopartida;
class PlayState : public GameState
{
public:
	PlayState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, int  coins, bool ab[6]=nullptr, bool died = false );

	virtual void init();
	virtual void update();

	void createMap();
	void createPlayer(const Config& playerConfig);
	void createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType);

	Entity* getBackgroundLevel();

private:
	Entity* backgroundLevel;
	SDL_Rect cam;
	MapProcedural* map;
	Vector2D camLimits;
	Fade* fadeComp;
	int coinsAfterRespawn;
	std::array<bool, 6> abilitiesAux;
	bool saveAbilities;
};
