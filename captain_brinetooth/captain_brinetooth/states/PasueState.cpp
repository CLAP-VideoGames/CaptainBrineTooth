#include "PasueState.h"

PauseState::PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;


	cam.w = cam.w * 2;
	cam.h = cam.h * 2;
	stRend = stateToRender;
}

void PauseState::init(){
	auto* fondo = manager_->addEntity();

	SDL_Rect posImage;
	posImage.x = 0;
	posImage.y = 0;
	posImage.w = cam.w;
	posImage.h = cam.h;

	fondo->addComponent<Image>(&sdlutils().images().at("debug_square"), posImage);
}

void PauseState::render() const {
	stRend->render();
	GameState::render();
}

void PauseState::update() {

}
