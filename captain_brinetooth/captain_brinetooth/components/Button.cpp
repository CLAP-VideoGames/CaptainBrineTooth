#include "Button.h"

Button::Button(Texture* t, CallBackOnClick* c, App* g, SoundManager* snd) : tex(t){
	cboq = c;
	game = g;
	soundController = snd;
}

Button::~Button(){

}

void Button::init(){
}

void Button::update(){
	handleEvent();
	if (fade != nullptr){
		if(fade->getFadeOutComplete())
			cboq(game, soundController);
	}
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

			fade = entity_->getComponent<Fade>();
			if(fade != nullptr){
				fade->setState(Fade::STATE_FADE::Out);
			}
			else{
				cboq(game, soundController);
			}
			return true;
		}

	}
	return false;
}

void Button::changeTex(Texture* newTexture)
{
	tex = newTexture;
}

void Button::changeTexColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetTextureColorMod(tex->getTexture(), r, g, b);
	SDL_SetTextureAlphaMod(tex->getTexture(), a);
}
