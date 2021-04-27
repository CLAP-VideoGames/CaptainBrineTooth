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
	std::pair<Vector2D, Vector2D> size = { Vector2D(500 * App::camera_Zoom_Out, 50 * App::camera_Zoom_Out), Vector2D(100 * App::camera_Zoom_Out, 100 * App::camera_Zoom_Out) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.25 * App::camera_Zoom_Out));
	Texture* textures[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* volume = OptionsState::createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume());
	panel.push_back(volume);

	// Efectos de sonido
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.55 * App::camera_Zoom_Out));
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* effects = OptionsState::createEffects(manager_, pos, size, textures, manager_->getSoundMngr()->EffectsVolume());
	panel.push_back(effects);

	// Brillo
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.85 * App::camera_Zoom_Out));
	Texture* textures2[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* brightness = OptionsState::createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()));
	panel.push_back(brightness);

	// Boton Volver
	float multiplier = App::camera_Zoom_Out;
	pos = Vector2D((cam.w * 0.135f), cam.h * 0.85);
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
	//BOTON BACKMENU---------
	float multiplier = App::camera_Zoom_Out;
	Vector2D pos = Vector2D((cam.w * 0.2f), cam.h * 0.5);
	createButton(&sdlutils().images().at("menu"), multiplier, pos, backToMenu);
	//BOTON OPTIONS---------
	pos = Vector2D((cam.w * 0.8f), cam.h * 0.5);
	createButton(&sdlutils().images().at("opciones"), multiplier, pos, pushOptionsPanel);
	//BOTON RESUME---------
	pos = Vector2D((cam.w * 0.5f), cam.h * 0.2);
	createButton(&sdlutils().images().at("continuar"), multiplier, pos, backToGame);
}

void PauseState::createButton(Texture* t, float& multiplier, Vector2D& pos_, void(*callback)(App*, SoundManager*)) {
	Vector2D size = Vector2D(t->width() * multiplier / 1.2, t->height() * multiplier / 1.2);
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
