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
	Transform* t = entity_->getComponent<Transform>();

	Vector2D aux = t->getPos();
	SDL_Rect dest = build_sdlrect(aux, t->getW(), t->getH());

	tex->render(dest);

	if (sdlutils().getDebug()){
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &dest);
	}
}

bool Button::handleEvent(){
	Transform* t = entity_->getComponent<Transform>();
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second};
	SDL_Rect dest = build_sdlrect(t->getPos(), t->getW(), t->getH());
	//Así no queda tan feo :D, no me grites David Please
	if (SDL_PointInRect(&mouseP, &dest) == SDL_TRUE){
		if (ih().mouseButtonEvent()){
			//game->getStateMachine()->currentState()->getMngr()->getSoundMngr();
			cboq(game, soundController);
			return true;
		}

	}
	return false;
}