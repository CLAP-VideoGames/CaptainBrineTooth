#pragma once
#include "GameState.h"
#include "box2d.h"
class Entity;

class PauseState :public GameState {
public:
	PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual void init();
	virtual void render() const;
	virtual void update();

	static void backToGame(App* app, SoundManager* snd);
	static void backToMenu(App* app, SoundManager* snd);
	GameState* stRend;

private:
	SDL_Rect cam;
};