#include "PasueState.h"

PauseState::PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;


	cam.w = cam.w * 2;
	cam.h = cam.h * 2;
	stRend = stateToRender;
}

void PauseState::init(){
	auto* fondo = manager_->addEntity();

	SDL_Rect posImage = build_sdlrect(0,0, cam.w, cam.h);

	fondo->addComponent<Image>(&sdlutils().images().at("debug_square"), posImage, "pausa");

	auto* subirV = createBasicEntity(Vector2D((cam.w / 3.1) + 25, cam.h*0.5 ), Vector2D(cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2)), 0.0f, Vector2D(0, 0));
	subirV->addComponent<Button>(&sdlutils().images().at("volverMenu"), backToGame, app, manager_->getSoundMngr());
}

void PauseState::render() const {
	stRend->render();
	GameState::render();
}

void PauseState::update() {
	GameState::update();

	for (Entity* b : manager_->getEnteties()) {
		Button* but = b->getComponent<Button>();
		if (but != nullptr && but->handleEvent())
		{
			break;
		}
	}
}

void PauseState::backToGame(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	sM->popState();
}
