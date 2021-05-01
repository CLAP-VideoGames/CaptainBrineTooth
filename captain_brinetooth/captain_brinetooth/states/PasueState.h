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

	//CallBacks
	static void backToGame(App* app, SoundManager* snd);
	static void backToMenu(App* app, SoundManager* snd);
	static void pushOptionsPanel(App* app, SoundManager* snd);
	static void pushPausePanel(App* app, SoundManager* snd);
	//Creacion de Paneles
	void createButton(Texture* t, float& sizeFactor, Vector2D& pos,void(*callback)(App*, SoundManager*));
	void createOptionPanel();
	void createBasePanel();

private:
	void clearPanel();
	std::vector<Entity*> panel;
	SDL_Rect cam;
	GameState* stRend;
};