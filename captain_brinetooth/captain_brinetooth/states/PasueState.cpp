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
	//zoom 2	std::pair<Vector2D, Vector2D> size = { Vector2D(500 * App::camera_Zoom_Out, 50 * App::camera_Zoom_Out), Vector2D(100 * App::camera_Zoom_Out, 100 * App::camera_Zoom_Out) };
	//zoom 2	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.25 * App::camera_Zoom_Out));
	
	std::pair<Vector2D, Vector2D> size = { Vector2D(900, 100), Vector2D(200, 200) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.2));
	
	Texture* textures[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* volume = OptionsState::createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume(), app);
	panel.push_back(volume);

	// Efectos de sonido
	//zoom 2	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.55 * App::camera_Zoom_Out));
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.5));
	
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };

	Entity* effects = OptionsState::createEffects(manager_, pos, size, textures, manager_->getSoundMngr()->EffectsVolume(), app);
	panel.push_back(effects);

	// Brillo
	//zoom 2 pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.85 * App::camera_Zoom_Out));
	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.75));
	
	Texture* textures2[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* brightness = OptionsState::createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()), app);
	panel.push_back(brightness);

	// Boton Volver
	float multiplier = 0.5;
	//zoom 2	pos = Vector2D((cam.w * 0.135f), cam.h * 0.85);
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
	SDL_Rect rectPos;
	rectPos.x = 0;
	rectPos.y = 0;
	rectPos.w = cam.w;
	rectPos.h = cam.h;

	float sizeFactor = 0.9;
	//Vector2D(cam.w / 1.9, cam.h * 0.07);
	//BOTON BACKMENU---------

	Vector2D pos = Vector2D((cam.w * 0.75f), cam.h * 0.5);

	createButton(&sdlutils().images().at("menu"), sizeFactor, pos, backToMenu);
	//BOTON OPTIONS---------
	pos = Vector2D((cam.w * 0.25f), cam.h * 0.5);
	createButton(&sdlutils().images().at("opciones"), sizeFactor, pos, pushOptionsPanel);
	//BOTON RESUME---------
	pos = Vector2D((cam.w * 0.5f), cam.h * 0.2);
	createButton(&sdlutils().images().at("continuar"), sizeFactor, pos, backToGame);
}

void PauseState::createButton(Texture* imageTexture, float& sizeFactor, Vector2D& pos, void(*callback)(App*, SoundManager*)) {
	float factor_ = app->getCameraZooOut();
	SDL_Rect rectPos;
	rectPos.x = 0;
	rectPos.y = 0;
	rectPos.w = cam.w;
	rectPos.h = cam.h;
	
	rectPos = GameState::ScaleSDL_Rect(imageTexture, pos, factor_, sizeFactor, true);
	
	auto* button = createBasicEntity(Vector2D(rectPos.x, rectPos.y), Vector2D(rectPos.w, rectPos.h), 0.0f, Vector2D(0, 0));
	button->addComponent<Button>(imageTexture, callback, app, manager_->getSoundMngr());

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
