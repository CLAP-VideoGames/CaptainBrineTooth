#pragma once


#include "../states/GameState.h"
#include "../states/StateMachine.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../components/SoundManager.h"


using CallBackOnClick = void(App* g, SoundManager* snd);

class Button : public Component {
public:
	Button(Texture* t, CallBackOnClick* c, App* g, SoundManager* snd) : tex(t)
	{
		cboq = c;
		game = g;
		soundController = snd;
	}

	virtual ~Button() {
	}

	void render();
	bool handleEvent(); 

protected:
	CallBackOnClick* cboq;

private:
	Texture* tex;
	SoundManager* soundController;
	App* game;

};
