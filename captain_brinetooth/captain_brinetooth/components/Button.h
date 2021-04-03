#pragma once


#include "../states/GameState.h"
#include "../states/GameStateMachine.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"

using CallBackOnClick = void();

class Button : public Component {
public:
	Button(Texture* t, CallBackOnClick* c) : tex(t)
	{
		cboq = c;
	}

	virtual ~Button() {
	}

	void render();
	bool handleEvent(SDL_Event& event); 

protected:
	CallBackOnClick* cboq;

private:
	Texture* tex;

};
