#include "Rectangle.h"

Rectangle::Rectangle(): Rectangle(build_sdlcolor(0xffffffff)) {

	}

Rectangle::Rectangle(SDL_Color color) :
	color_(color), //
	tr_(nullptr) {
}

Rectangle::~Rectangle()
{
}

void Rectangle::init() {
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

}

void Rectangle::render() {

	SDL_SetRenderDrawColor(sdlutils().renderer(), COLOREXP(color_));

	SDL_Rect rect = build_sdlrect(tr_->getPos(), tr_->getW(), tr_->getH());

	SDL_RenderFillRect(sdlutils().renderer(), &rect);
}