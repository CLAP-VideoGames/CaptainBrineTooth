#include "Fade.h"

Fade::Fade(const Vector2D& size, const Vector2D& pos, const int& timeIn, const int& timeOut, STATE_FADE state){
	dest = build_sdlrect(pos.getX(), pos.getY(), size.getX(), size.getY());
	colorFade = build_sdlcolor(0, 0, 0, 255);
	timeIn_ = timeIn;
	timeOut_ = timeOut;
	state_ = state;
}

Fade::~Fade(){
}

void Fade::init(){
}

void Fade::update(){
	int alpha;
	if (state_ == STATE_FADE::In){
		alpha = sdlutils().lerpPrecise(colorFade.a, 0, 0.01);

	}
	else{
		alpha = sdlutils().lerpPrecise(colorFade.a, 255, 0.3);
	}

	colorFade.a = alpha;
}


void Fade::render(){
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(colorFade));
	SDL_RenderFillRect(sdlutils().renderer(), &dest);
}