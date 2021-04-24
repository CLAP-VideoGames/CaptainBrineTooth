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
	snd->resumeMainMusic();
	StateMachine* sM = app->getStateMachine();
	sM->popState();
}

void PauseState::backToMenu(App* app, SoundManager* snd) {
	snd->ChangeMainMusic("Menu");
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
	// Volumen
	std::pair<Vector2D, Vector2D> size = { Vector2D(900, 100), Vector2D(200, 200) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.2));
	Texture* textures[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* volume = OptionsState::createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume());
	panel.push_back(volume);

	// Efectos de sonido
	size = { Vector2D(900, 100), Vector2D(200, 200) };
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.5));
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* effects = OptionsState::createEffects(manager_, pos, size, textures, manager_->getSoundMngr()->EffectsVolume());
	panel.push_back(effects);

	// Brillo
	size = { Vector2D(900, 100), Vector2D(200, 200) };
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.75));
	Texture* textures2[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* brightness = OptionsState::createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()));
	panel.push_back(brightness);

	// Boton Volver
	float multiplier = 2;
	pos = Vector2D((cam.w * 0.1f), cam.h * 0.9);
	createButton(&sdlutils().images().at("volverMenu"), multiplier, pos, pushPausePanel);
}

void PauseState::pushPausePanel(App* app, SoundManager* snd) {
	PauseState* state = dynamic_cast<PauseState*>(app->getStateMachine()->currentState());
	if (state != nullptr) {
		state->clearPanel();
		state->createBasePanel();
	}
}

void PauseState::createBasePanel() {
	//Texture* t;
	//BOTON BACKMENU---------
	//t = &sdlutils().msgs().at("BackMenu");
	float multiplier = 2;
	Vector2D pos = Vector2D((cam.w * 0.2f), cam.h * 0.5);
	createButton(&sdlutils().images().at("menu"), multiplier, pos, backToMenu);
	//BOTON OPTIONS---------
	//t = &sdlutils().msgs().at("Options");opciones
	multiplier = 2;
	pos = Vector2D((cam.w * 0.8f), cam.h * 0.5);
	createButton(&sdlutils().images().at("opciones"), multiplier, pos, pushOptionsPanel);
	//BOTON RESUME---------
	//t = &sdlutils().msgs().at("Resume");
	multiplier = 2;
	pos = Vector2D((cam.w * 0.5f), cam.h * 0.2);
	createButton(&sdlutils().images().at("continuar"), multiplier, pos, backToGame);
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
	for (Entity* ent : panel){
		Slider* slider = ent->getComponent<Slider>();
		//Desactivamos las entidades auxiliares del Slider, en caso de serlo.
		if (slider != nullptr) slider->desactivateSlider();
		ent->setActive(false);
	}
	panel.clear();
}
