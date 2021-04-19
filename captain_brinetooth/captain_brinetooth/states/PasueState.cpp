#include "PasueState.h"
#include "OptionsState.h"
#include "MenuState.h"

PauseState::PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd) {
	stRend = stateToRender;
	cam = a->camera;
	cam.w = cam.w * a->getCameraZooOut();
	cam.h = cam.h * a->getCameraZooOut();
}

void PauseState::init() {
	auto* fondo = manager_->addEntity();
	SDL_Rect posImage = build_sdlrect(0, 0, cam.w, cam.h);
	fondo->addComponent<Image>(&sdlutils().images().at("debug_square"), posImage, "pausa");
	createBasePanel();
}

void PauseState::render() const {
	stRend->render();
	GameState::render();
}

void PauseState::update() {
	GameState::update();
}

void PauseState::backToGame(App* app, SoundManager* snd) {
	StateMachine* sM = app->getStateMachine();
	sM->popState();
}

void PauseState::backToMenu(App* app, SoundManager* snd) {
	StateMachine* sM = app->getStateMachine();
	Manager* mngr = sM->currentState()->getMngr();
	sM->popState();
	sM->changeState(new MenuState(app, mngr->getWorld(), mngr->getSoundMngr()));
}

void PauseState::pushOptionsPanel(App* app, SoundManager* snd) {
	PauseState* state = dynamic_cast<PauseState*>(app->getStateMachine()->currentState());
	if (state != nullptr) {
		state->clearPanel();
		state->createOptionPanel();
	}
}

void PauseState::createOptionPanel() {
	std::pair<Vector2D, Vector2D> size = { Vector2D(900, 50), Vector2D(100, 100) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.3));
	Texture* textures[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* volume = OptionsState::createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->GeneralVolume());
	panel.push_back(volume);

	size = { Vector2D(900, 50), Vector2D(100, 100) };
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.5));
	Texture* textures2[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* brightness = OptionsState::createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()));
	panel.push_back(brightness);

	Texture* t = &sdlutils().msgs().at("Return");
	float multiplier = 2;
	pos = Vector2D((cam.w * 0.5f), cam.h * 0.8);
	createButton(t, multiplier, pos, pushPausePanel);
}

void PauseState::pushPausePanel(App* app, SoundManager* snd) {
	PauseState* state = dynamic_cast<PauseState*>(app->getStateMachine()->currentState());
	if (state != nullptr) {
		state->clearPanel();
		state->createBasePanel();
	}
}

void PauseState::createBasePanel() {
	Texture* t;
	//BOTON BACKMENU---------
	t = &sdlutils().msgs().at("BackMenu");
	float multiplier = 2;
	Vector2D pos = Vector2D((cam.w * 0.5f), cam.h * 0.8);
	createButton(t, multiplier, pos, backToMenu);
	//BOTON OPTIONS---------
	t = &sdlutils().msgs().at("Options");
	multiplier = 2;
	pos = Vector2D((cam.w * 0.5f), cam.h * 0.5);
	createButton(t, multiplier, pos, pushOptionsPanel);
	//BOTON RESUME---------
	t = &sdlutils().msgs().at("Resume");
	multiplier = 2;
	pos = Vector2D((cam.w * 0.5f), cam.h * 0.2);
	createButton(t, multiplier, pos, backToGame);
	//backMenu->addComponent<Button>(&sdlutils().images().at("volverMenu"), backToMenu, app, manager_->getSoundMngr());
}

void PauseState::createButton(Texture* t, float& multiplier, Vector2D& pos_, void(*callback)(App*, SoundManager*)) {
	Vector2D size = Vector2D(t->width() * multiplier, t->height() * multiplier);
	Vector2D pos = Vector2D(pos_.getX() - (size.getX() / (2 * getApp()->getCameraZooOut())), pos_.getY());
	//Vector2D pos = Vector2D((cam.w * 0.5f) - (size.getX() / (2 * getApp()->getCameraZooOut())), cam.h * 0.5);
	auto* button = createBasicEntity(pos, size, 0.0f, Vector2D(0, 0));
	button->addComponent<Button>(t, callback, app, manager_->getSoundMngr());

	panel.push_back(button);
}

void PauseState::clearPanel() {
	for (Entity* ent : panel) ent->setActive(false);
	panel.clear();
}
