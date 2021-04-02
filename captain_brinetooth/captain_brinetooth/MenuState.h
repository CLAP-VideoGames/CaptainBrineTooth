#pragma once
#include "GameState.h"
#include "components/SoundManager.h"
#include "box2d.h"
class Entity;

class MenuState : public GameState
{
public:
	MenuState(Game* a,b2World* mundo);
	void addStateEntityMenu();
	void setSoundManager(SoundManager* sound) { snd = sound; }
	static void changeToGame();
	static void PlayGaviota();
	void update();

private:
	SoundManager* snd;
};

