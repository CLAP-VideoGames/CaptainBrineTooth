#pragma once
#include "GameState.h"
class Entity;
struct infopartida;
class PlayState : public GameState
{
public:
	PlayState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, bool saved, int  coins, bool ab[6]=nullptr, bool died = false );
	virtual ~PlayState();

	virtual void init();
	virtual void update();

	void createMap();
	void createPlayer(const Config& playerConfig);
	void createWeaponGiver(const Config& weaponGiverConfig, const int& weaponType);
	void createBoxTest(const Config& entityConfig);
	void createElfShark(const Config& entityConfig);
	void createFlowerJellyHat(const Config& entityConfig);
	void createSaveDataandSTate();

	Entity* getBackgroundLevel();

private:
	void dataInRespawn(bool died,int coins, bool ab[6]);


	Entity* backgroundLevel;
	SDL_Rect cam;
	MapProcedural* map;
	Vector2D camLimits;
	bool save;
	Fade* fadeComp;
	int coinsAfterRespawn;
	std::array<bool, 6> abilitiesAux;
	bool saveAbilities;
};
