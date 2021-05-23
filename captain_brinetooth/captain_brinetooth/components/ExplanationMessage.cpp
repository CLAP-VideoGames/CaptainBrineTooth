#include "ExplanationMessage.h"

ExplanationMessage::ExplanationMessage()
{
}

ExplanationMessage::~ExplanationMessage()
{
}

void ExplanationMessage::init(){
	tr = entity_->getComponent<Transform>();
}

void ExplanationMessage::update() {
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second };
	SDL_Rect dest = build_sdlrect(tr->getPos(), tr->getW(), tr->getH());
	if (SDL_PointInRect(&mouseP, &dest) == SDL_TRUE) {

	}
}

void ExplanationMessage::render()
{
}
