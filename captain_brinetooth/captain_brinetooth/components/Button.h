#pragma once


#include "../states/GameState.h"
#include "../states/StateMachine.h"
#include "Transform.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/InputHandler.h"
#include "../ecs/Entity.h"
#include "../ecs/SoundManager.h"
#include "../sdlutils/SDLUtils.h"

#include "..//components/Fade.h"


using CallBackOnClick = void(App* g, SoundManager* snd);

class Button : public Component {
public:
	Button(Texture* t, CallBackOnClick* c, App* g, SoundManager* snd);

	virtual ~Button();

	void init() override;

	void update() override;
	void render() override;
	bool handleEvent(); 

protected:
	CallBackOnClick* cboq;

private:
	Fade* fade;
	Texture* tex;
	SoundManager* soundController;
	App* game;

};
