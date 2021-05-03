#include "Button.h"

Button::Button(Texture* t, CallBackOnClick* c, App* g, SoundManager* snd) : tex(t){
	cboq = c;
	game = g;
	soundController = snd;
}

Button::~Button(){
}

void Button::update(){
	handleEvent();
}

void Button::render(){
	Transform* trans = entity_->getComponent<Transform>();

	Vector2D aux = trans->getPos();
	SDL_Rect dest = build_sdlrect(aux, trans->getW(), trans->getH());

	tex->render(dest);

	if (sdlutils().getDebug()){
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &dest);
	}
}

bool Button::handleEvent(){
	Transform* tr = entity_->getComponent<Transform>();
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second};
	SDL_Rect dest = build_sdlrect(tr->getPos(), tr->getW(), tr->getH());
	if (SDL_PointInRect(&mouseP, &dest) == SDL_TRUE){
		if (ih().mouseButtonEvent()){
			cboq(game, soundController);
			return true;
		}

	}
	return false;
}