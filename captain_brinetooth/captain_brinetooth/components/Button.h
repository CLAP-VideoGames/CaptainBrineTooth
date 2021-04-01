#pragma once


#include "../GameState.h"
#include "../GameStateMachine.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"



using CallBackOnClick = void(Game* game);

class Button : public Component {
public:
	Button(Texture* t, CallBackOnClick* c, Game* g) : tex(t), cboq(c), game(g)
	{
	}

	virtual ~Button() {
	}

	void render();
	bool handleEvent(SDL_Event& event); 

protected:
	CallBackOnClick* cboq;

private:
	Texture* tex;
	Game* game;
};
