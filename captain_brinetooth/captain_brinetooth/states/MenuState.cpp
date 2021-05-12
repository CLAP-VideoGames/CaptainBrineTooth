#include "MenuState.h"
#include "OptionsState.h"
#include "PlayState.h"
#include "TestZoneState.h"
#include "TutorialState.h"

MenuState::MenuState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;
}

void MenuState::init() {
	auto* fondo = manager_->addEntity();
	SDL_Rect rectPos;
	rectPos.x = 0;
	rectPos.y = 0;
	rectPos.w = cam.w;
	rectPos.h = cam.h;

	//Background
	std::deque<std::pair<const char*, std::pair<bool, int>>> videos;
																//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("waves").c_str(), {true, 25} };
	videos.push_back(video__);
	auto* video = manager_->addEntity();
	video->addComponent<VideoPlayer>(videos);
	if (manager_->getSoundMngr()->GeneralVolume() > manager_->getSoundMngr()->PauseVolume())
	{
		manager_->getSoundMngr()->setGeneralVolume(manager_->getSoundMngr()->GeneralVolume());
	}
	else
	{
		manager_->getSoundMngr()->setGeneralVolume(manager_->getSoundMngr()->PauseVolume());
	}

	manager_->getSoundMngr()->playMainMusic();

	// Titulo
	auto* title = manager_->addEntity();
	int w = (int)sdlutils().width() * App::camera_Zoom_Out * 0.3;
	int h = (int)w * 0.6;	//mantener aspect ratio
	int x = (int)((App::camera.w - w) * 0.5);
	int y = (int)((App::camera.h - h) * 0.05);
	title->addComponent<Image>(&sdlutils().images().at("menu_titulo"), SDL_Rect{ x, y, w, h }, "title");
#pragma region Botones
	//Atributos de los botones
	y = (int)((y + h * 0.75));
	w = (int)sdlutils().width() * 0.2 * App::camera_Zoom_Out;
	h = (int)w * 0.4;
	x = (int)((App::camera.w - w) * 0.5);

	//Boton Cargar
	if (!saved_game) {
		auto* botonCargar = manager_->addEntity();
		w *= 0.8;
		h *= 0.8;
		x = (int)((App::camera.w - w) * 0.5);
		y = (int)((y + h * 1.2));
		//y = (int)((App::camera.h - h) * (0.48 + 0.22 * nBoton));
		botonCargar->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), w, h, 0.0f);
		botonCargar->addComponent<Button>(&sdlutils().images().at("continuar_menu"), changeToGame, app, manager_->getSoundMngr());
		w *= 1.25;
		h *= 1.25;
		x = (int)((App::camera.w - w) * 0.5);
	}
	


	//Creditos
	auto* creditos = manager_->addEntity();
	int x_ = (int)((App::camera.w - w) * 0.02);
	int y_ = (int)((App::camera.h - h) * 0.98);
	creditos->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), w * 0.7, h * 0.7, 0.0f);
	creditos->addComponent<Button>(&sdlutils().images().at("creditos_menu"), salirMenu, app, manager_->getSoundMngr());

	// Boton de salir
	auto* salir = manager_->addEntity();
	x_ = (int)((App::camera.w - w) * 1.05);
	y_ = (int)((App::camera.h - h) * 0.98);
	salir->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), w*0.7, h*0.7, 0.0f);
	salir->addComponent<Button>(&sdlutils().images().at("salir_menu"), salirMenu, app, manager_->getSoundMngr());

	// Opciones
	auto* opciones = manager_->addEntity();
	y = (int)((y + (h * 1.2 * 2)));
	opciones->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), w, h, 0.0f);
	opciones->addComponent<Button>(&sdlutils().images().at("ajustes_menu"), changeToOptions, app, manager_->getSoundMngr());

	// Boton Iniciar
	auto* botonIniciar = manager_->addEntity();
	y = (int)((y - ((h * 1.2))));
	botonIniciar->addComponent<Transform>(Vector2D(x, y), Vector2D(0, 0), w, h, 0.0f);
	botonIniciar->addComponent<Button>(&sdlutils().images().at("nueva_partida_menu"), changeToTutorial, app, manager_->getSoundMngr());
	botonIniciar->addComponent<Fade>(Vector2D(cam.w, cam.h), Vector2D(0, 0), 1500, 1000);

#pragma endregion
}

void MenuState::changeToTutorial(App* app, SoundManager* snd) {
	snd->playSoundEffect("sonido_barco", 0);

	StateMachine* sM = app->getStateMachine();
	sM->changeState(new TutorialState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::changeToGame(App* app, SoundManager* snd) {
	snd->playSoundEffect("sonido_barco", 0);
	
	StateMachine* sM = app->getStateMachine();
	sM->changeState(new PlayState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::changeToOptions(App* app, SoundManager* snd) {
	snd->playSoundEffect("gaviota",0);

	StateMachine* sM = app->getStateMachine();
	sM->pushState(new OptionsState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::salirMenu(App* game, SoundManager* snd){
	game->exitGame();
}