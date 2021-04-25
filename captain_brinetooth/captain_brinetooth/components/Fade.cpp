#include "Fade.h"
#include "../game/App.h"

Fade::Fade(const Vector2D& size, const Vector2D& pos, const int& timeIn, const int& timeOut, STATE_FADE state){
	dest = build_sdlrect(pos.getX(), pos.getY(), size.getX(), size.getY());
	colorFade = build_sdlcolor(0, 0, 0, 255);

	timeIn_ = timeIn * App::FPS;
	timeOut_ = timeOut* App::FPS;
	state_ = state;
}

Fade::~Fade(){
}

void Fade::init(){
	percentageIn = ((float)255 / (float)timeIn_);
	percentageOut = ((float)255 / (float)timeOut_);
}

void Fade::update(){
	if (state_ == STATE_FADE::In && !fadeInComplete){
		colorFade.a = floor(sdlutils().lerpPrecise(colorFade.a, 0, percentageIn));
		if (colorFade.a <= 0)
			fadeInComplete = true;

	}
	else if(state_ == STATE_FADE::Out && !fadeOutComplete){
		colorFade.a = ceil(sdlutils().lerpPrecise(colorFade.a, 255, percentageOut));
		if(colorFade.a >= 255)
			fadeOutComplete = true;
	}
}


void Fade::render(){
	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(colorFade));
	SDL_RenderFillRect(sdlutils().renderer(), &dest);
}