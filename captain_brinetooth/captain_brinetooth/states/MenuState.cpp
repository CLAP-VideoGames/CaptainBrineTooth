#include "MenuState.h"
#include "../states/OptionsState.h"
#include "../states/PlayState.h"
#include "../states/TestZoneState.h"

MenuState::MenuState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;
	//No tengo ni idea pero si no es chikito
	cam.w = cam.w * a->getCameraZooOut();
	cam.h = cam.h * a->getCameraZooOut();
}

void MenuState::init() {
	auto* fondo = manager_->addEntity();
	SDL_Rect rectPos;
	rectPos.x = 0;
	rectPos.y = 0;
	rectPos.w = cam.w;
	rectPos.h = cam.h;

	fondo->addComponent<Image>(&sdlutils().images().at("fondoMenu"), rectPos, "fondoMenu");

	manager_->getSoundMngr()->setGeneralVolume(63.5);
	manager_->getSoundMngr()->playMainMusic();

	float sizeFactor;
	// Titulo
	auto* title = manager_->addEntity();
	Texture* imageTexture = &sdlutils().images().at("titulo");
	sizeFactor = 0.4;
	float factor_ = app->getCameraZooOut();

	rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(cam.w/1.9,cam.h * 0.07), factor_, sizeFactor, true);
	title->addComponent<Image>(imageTexture, rectPos, "titulo");


	//// Boton Iniciar
	sizeFactor = 1;
	imageTexture = &sdlutils().images().at("jugar");
	rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(cam.w/2 , cam.h* 0.4), factor_, sizeFactor, true);

	auto botonIniciar = manager_->addEntity();
	botonIniciar->addComponent<Transform>(Vector2D(rectPos.x, rectPos.y), Vector2D(0, 0), rectPos.w, rectPos.h, 0.0f);
	botonIniciar->addComponent<Button>(&sdlutils().images().at("jugar"), changeToGame, app, manager_->getSoundMngr());

	//// Opciones
	imageTexture = &sdlutils().images().at("opciones");
	rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(cam.w / 2, cam.h * 0.6), factor_, sizeFactor, true);

	auto opciones = manager_->addEntity();
	opciones->addComponent<Transform>(Vector2D(rectPos.x, rectPos.y), Vector2D(0, 0), rectPos.w, rectPos.h, 0.0f);
	opciones->addComponent<Button>(imageTexture, changeToOptions, app, manager_->getSoundMngr());



	//// Boton de salir
	imageTexture = &sdlutils().images().at("salir");
	rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(cam.w / 2, cam.h * 0.8), factor_, sizeFactor, true);
	
	auto salir = manager_->addEntity();
	salir->addComponent<Transform>(Vector2D(rectPos.x, rectPos.y), Vector2D(0, 0), rectPos.w, rectPos.h, 0.0f);
	salir->addComponent<Button>(imageTexture, salirMenu, app, manager_->getSoundMngr());
}

void MenuState::changeToGame(App* app, SoundManager* snd) {
	snd->ChangeMainMusic("Nivel1");

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