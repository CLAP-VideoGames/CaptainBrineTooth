#pragma once


#include "../GameState.h"
#include "../GameStateMachine.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../components/SoundManager.h"


using CallBackOnClick = void(Game* g, SoundManager* snd);

class Button : public Component {
public:
	Button(Texture* t, CallBackOnClick* c, Game* g, SoundManager* snd) : tex(t)
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
	Game* game;

};
