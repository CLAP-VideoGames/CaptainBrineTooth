#pragma once
#include "GameState.h"
#include "box2d.h"

class Entity;

class OptionsState : public GameState
{
public:

	OptionsState(App* app, b2World* mundo, SoundManager* snd);
	virtual void init();
	static void subirVolumen(App* app, SoundManager* snd);
	static void bajarVolumen(App* app, SoundManager* snd);
	static void volverMenu(App* app, SoundManager* snd);


	void update();
	void setSoundController(SoundManager* snd) { soundController = snd; }

private:
	SDL_Rect cam;
};

