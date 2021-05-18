#pragma once


#include "../states/GameState.h"
#include "../states/StateMachine.h"
#include "Transform.h"
#include "Image.h"
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

	void setTex(Texture* newTexture);
	void setTexColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	void canFadeOut(bool b);

protected:
	CallBackOnClick* cboq;

private:
	bool selected;
	bool apply_offset;
	bool mouseOnButton;
	bool canFadeOut_;
	//Controlador para que el evento que acciona el botón solo se reproduzca una vez con el fadeOut
	bool eventTriggered_;
	float offset_pressed = 5 * App::camera_Zoom_Out;
	Fade* fade;
	Texture* tex;
	Image* img_;
	SoundManager* soundController;
	App* game;

};
