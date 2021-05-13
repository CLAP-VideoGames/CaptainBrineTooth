#pragma once
#include "GameState.h"
#include "box2d.h"


class Entity;

struct infoPartida
{
	int weapon1;
	int weapon2;
	float playerLife;
	//etc..
};
static infoPartida partida;
class PauseState :public GameState {
public:
	PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, infoPartida info);
	virtual void init();
	virtual void render() const;
	virtual void update();

	//CallBacks
	static void backToGame(App* app, SoundManager* snd);
	static void backToMenu(App* app, SoundManager* snd);
	static void pushOptionsPanel(App* app, SoundManager* snd);
	static void pushPausePanel(App* app, SoundManager* snd);
	static void quitGame(App* game, SoundManager* snd);
	static void savePartida(infoPartida info);
	//Creacion de Paneles
	void createButton(Texture* t, Vector2D pos, Vector2D size,void(*callback)(App*, SoundManager*));
	void createOptionPanel();
	void createBasePanel();

private:
	void clearPanel();
	std::vector<Entity*> panel;
	SDL_Rect cam;
	GameState* stRend;

};