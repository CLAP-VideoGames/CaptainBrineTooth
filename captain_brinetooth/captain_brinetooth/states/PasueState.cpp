#include "PasueState.h"
#include "OptionsState.h"
#include "MenuState.h"

PauseState::PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	stRend = stateToRender;
	cam = a->camera;
	cam.w = cam.w * a->getCameraZooOut();
	cam.h = cam.h * a->getCameraZooOut();
}

void PauseState::init(){
	auto* fondo = manager_->addEntity();

	SDL_Rect posImage = build_sdlrect(0,0, cam.w, cam.h);

	fondo->addComponent<Image>(&sdlutils().images().at("debug_square"), posImage, "pausa");

	Vector2D size = Vector2D(700, 300);
	Vector2D pos = Vector2D((cam.w * 0.5f) - (size.getX()/ (2 * getApp()->getCameraZooOut())), cam.h * 0.8);
	auto* backGame = createBasicEntity(pos, size, 0.0f, Vector2D(0, 0));
	backGame->addComponent<Button>(&sdlutils().msgs().at("BackMenu"), backToMenu, app, manager_->getSoundMngr());
	
	size = Vector2D(500, 300);
	pos = Vector2D((cam.w * 0.5f) - (size.getX() / (2 * getApp()->getCameraZooOut())), cam.h * 0.5);
	auto* backMenu = createBasicEntity(pos, size, 0.0f, Vector2D(0, 0));
	backMenu->addComponent<Button>(&sdlutils().msgs().at("Options"), backToMenu, app, manager_->getSoundMngr());
	
	size = Vector2D(700, 300);
	pos = Vector2D((cam.w * 0.5f) - (size.getX()/ (2 * getApp()->getCameraZooOut())), cam.h * 0.2);
	auto* resume = createBasicEntity(pos, size, 0.0f, Vector2D(0, 0));
	resume->addComponent<Button>(&sdlutils().msgs().at("Resume"), backToGame, app, manager_->getSoundMngr());
	//backMenu->addComponent<Button>(&sdlutils().images().at("volverMenu"), backToMenu, app, manager_->getSoundMngr());
}

void PauseState::render() const {
	stRend->render();
	GameState::render();
}

void PauseState::update() {
	GameState::update();
}

void PauseState::backToGame(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	sM->popState();
}

void PauseState::backToMenu(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	Manager* mngr = sM->currentState()->getMngr();
	sM->popState();
	sM->changeState(new MenuState(app, mngr->getWorld(), mngr->getSoundMngr()));
}

void PauseState::clearPanel(){
	for (Entity* ent : panel) ent->setActive(false);
	panel.clear();
}
