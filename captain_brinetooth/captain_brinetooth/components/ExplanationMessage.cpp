#include "ExplanationMessage.h"
#include "../sdlutils/SDLUtils.h"

ExplanationMessage::ExplanationMessage()
{
	size = Vector2D(320, 128);
	textures = {};
	render_ = false;
	currentTexture = nullptr;
}

ExplanationMessage::~ExplanationMessage()
{
}

void ExplanationMessage::init(){
	tr = entity_->getComponent<Transform>();
}

void ExplanationMessage::update() {
	mouseP = { ih().getMousePos().first, ih().getMousePos().second };
	SDL_Rect dest = build_sdlrect(tr->getPos(), tr->getW(), tr->getH());
	if (SDL_PointInRect(&mouseP, &dest) == SDL_TRUE) {
		render_ = true;
	}
	else{
		render_ = false;
	}
}

void ExplanationMessage::render(){
	if (render_){
		if (currentTexture != nullptr) {
			Vector2D pos = Vector2D(mouseP.x, mouseP.y);
			//SDL_Rect dest = build_sdlrect(pos, tr->getW(), tr->getH());
			SDL_Rect dest = build_sdlrect(pos, size.getX(), size.getY());
			currentTexture->render(dest);
		}
	}
}

void ExplanationMessage::push_backTexture(Texture* t){
	if (t != nullptr)
		textures.push_back(t);
}

void ExplanationMessage::setCurrentTexture(int i){
	if (i < textures.size()) 
		currentTexture = textures[i];
}

int ExplanationMessage::getSizeTextures(){
	return textures.size();
}
