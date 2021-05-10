#include "PasueState.h"
#include "OptionsState.h"
#include "MenuState.h"

PauseState::PauseState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd) {
	stRend = stateToRender;
	cam = a->camera;
}

void PauseState::init() {
	auto* fondo = manager_->addEntity();
	SDL_Rect posImage = build_sdlrect(0, 0, cam.w, cam.h);
	fondo->addComponent<Image>(&sdlutils().images().at("bg_pausa"), posImage, "pausa");
	createBasePanel();
}

void PauseState::render() const {
	stRend->render();
	GameState::render();
}

void PauseState::update() {
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			backToGame(app, getMngr()->getSoundMngr());
		}
	}
	GameState::update();
}

void PauseState::backToGame(App* app, SoundManager* snd) {
	snd->resumeMainMusic(false);
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
	


	//Tamaño de las texturas del Slider
	std::pair<Vector2D, Vector2D> size = { Vector2D(600, 30), Vector2D(95, 100) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.2));

	//Volumen
	Texture* textures[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* volume = OptionsState::createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume(), app);
	panel.push_back(volume);
	//Efectos
	pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.55));
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* effects = OptionsState::createEffects(manager_, pos, size, textures1, manager_->getSoundMngr()->EffectsVolume(), app);
	panel.push_back(effects);
	//Brillo
	pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.85));
	Texture* textures2[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	Entity* brightness = OptionsState::createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()), app);
	panel.push_back(brightness);

	// Back
	int w = (int)sdlutils().width() * 0.25 * App::camera_Zoom_Out * 0.65;
	int h = (int)w * 0.4;
	int x = (int)((App::camera.w - w) * 0.02);
	int y = (int)((App::camera.h - h) * 0.98);
	createButton(&sdlutils().images().at("atras_boton"), Vector2D(x,y), Vector2D(w,h), pushPausePanel);
}

void PauseState::pushPausePanel(App* app, SoundManager* snd) {
	PauseState* state = dynamic_cast<PauseState*>(app->getStateMachine()->currentState());
	if (state != nullptr) {
		state->clearPanel();
		state->createBasePanel();
	}
}

void PauseState::createBasePanel() {
	//Atributos de los botones
	int w = (int)sdlutils().width() * 0.25 * App::camera_Zoom_Out;
	int h = (int)w * 0.4;
	int x = (int)((App::camera.w - w) * 0.5);
	int y = (int)((App::camera.w - w) * 0.15);
	// Continue
	createButton(&sdlutils().images().at("continuar_menu"), Vector2D(x,y), Vector2D(w,h), backToGame);
	// Options
	w *= 0.8;
	h *= 0.8;
	x = (int)((App::camera.w - w) * 0.2);
	y = (int)((App::camera.h - h) * 0.6);
	createButton(&sdlutils().images().at("ajustes_menu"), Vector2D(x, y), Vector2D(w, h), pushOptionsPanel);
	//Back to menu
	x = (int)((App::camera.w - w) * 0.8);
	createButton(&sdlutils().images().at("menu_boton"), Vector2D(x, y), Vector2D(w, h), backToMenu);
	// Exit
	w *= 0.8;
	h *= 0.8;
	x = (int)((App::camera.w - w) * 0.98);
	y = (int)((App::camera.h - h) * 0.98);
	createButton(&sdlutils().images().at("salir_menu"), Vector2D(x, y), Vector2D(w, h), quitGame);
}

void PauseState::createButton(Texture* imageTexture, Vector2D pos, Vector2D size, void(*callback)(App*, SoundManager*)) {	
	auto* button = createBasicEntity(Vector2D(pos.getX(), pos.getY()), Vector2D(size.getX(), size.getY()), 0.0f, Vector2D(0, 0));
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
void PauseState::quitGame(App* game, SoundManager* snd) {
	game->exitGame();
}