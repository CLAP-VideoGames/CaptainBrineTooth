#pragma once
#include "GameState.h"
#include "box2d.h"

class Entity;

class MenuState : public GameState
{
public:

	MenuState(App* app, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual void init();
	static void changeToGame(App* app, SoundManager* snd);
	static void salirMenu(App* app, SoundManager* snd);
	static void changeToOptions(App* app, SoundManager* snd);
	static void changeToTutorial(App* app, SoundManager* snd);
	static void changeToCredits(App* app, SoundManager* snd);
	void checkSaveFile();

private:
	SDL_Rect cam;
	Fade* fadeComp;
};

