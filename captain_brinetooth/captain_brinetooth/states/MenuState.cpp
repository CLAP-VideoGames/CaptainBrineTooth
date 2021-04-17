#include "MenuState.h"
#include "../states/OptionsState.h"
#include "../states/PlayState.h"

MenuState::MenuState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd)
{
	cam = a->camera;

	//No tengo ni idea pero si no es chikito
	cam.w = cam.w * 2;
	cam.h = cam.h * 2;
}

void MenuState::init() {
	auto* fondo = manager_->addEntity();
	SDL_Rect posImage;
	posImage.x = 0;
	posImage.y = 0;
	posImage.w = cam.w;
	posImage.h = cam.h;

	fondo->addComponent<Image>(&sdlutils().images().at("fondoMenu"), posImage, "fondoMenu");

	manager_->getSoundMngr()->setGeneralVolume(63.5);
	manager_->getSoundMngr()->playMainMusic();


	// Titulo
	auto* title = manager_->addEntity();
	title->addComponent<Transform>(Vector2D((cam.w / 3.1) - 50, 80), Vector2D(0, 0), cam.w - (cam.w / 9), cam.h - (cam.h / 2), 0.0f);

	title->addComponent<Button>(&sdlutils().images().at("titulo"), changeToGame, app, manager_->getSoundMngr());

	// Posicion y altura del titulo para ajustar la posicion del siguiente boton
	Vector2D pos = title->getComponent<Transform>()->getPos();
	int alt = title->getComponent<Transform>()->getH();

	// Boton Iniciar
	auto botonIniciar = manager_->addEntity();
	botonIniciar->addComponent<Transform>(Vector2D(pos.getX() + 75, pos.getY() + alt / 1.5), Vector2D(0, 0), cam.w - (cam.w / 2.5), cam.h - (cam.h / 1.3), 0.0f);
	botonIniciar->addComponent<Button>(&sdlutils().images().at("jugar"), changeToGame, app, manager_->getSoundMngr());

	// Ajuste de la posicion y altura auxiliar para el siguiente boton
	pos = botonIniciar->getComponent<Transform>()->getPos();
	alt = botonIniciar->getComponent<Transform>()->getH();

	// Opciones
	auto opciones = manager_->addEntity();
	opciones->addComponent<Transform>(Vector2D(pos.getX(), pos.getY() + alt / 1.25), Vector2D(0, 0), cam.w - (cam.w / 2.5), cam.h - (cam.h / 1.3), 0.0f);
	opciones->addComponent<Button>(&sdlutils().images().at("opciones"), changeToOptions, app, manager_->getSoundMngr());

	// Ajuste de la posicion y altura auxiliar para el siguiente boton
	pos = opciones->getComponent<Transform>()->getPos();
	alt = opciones->getComponent<Transform>()->getH();

	// Boton de salir
	auto salir = manager_->addEntity();
	salir->addComponent<Transform>(Vector2D(pos.getX(), pos.getY() + alt / 1.25), Vector2D(0, 0), cam.w - (cam.w / 2.5), cam.h - (cam.h / 1.3), 0.0f);
	salir->addComponent<Button>(&sdlutils().images().at("salir"), salirMenu, app, manager_->getSoundMngr());
}

void MenuState::update() {

	GameState::update();

	for (Entity* b : manager_->getEnteties()){
		Button* but = b->getComponent<Button>();
		if (but != nullptr && but->handleEvent())
		{
			break;
		}
	}

}

void MenuState::changeToGame(App* app, SoundManager* snd) {
	snd->stopMusic();
	//snd->playSoundEffect("gaviota");

	StateMachine* sM = app->getStateMachine();
	sM->changeState(new PlayState(app, sM->currentState()->getMngr()->getWorld(), snd));
}

void MenuState::changeToOptions(App* app, SoundManager* snd) {
	snd->stopMusic();
	snd->playSoundEffect("gaviota");

	StateMachine* sM = app->getStateMachine();
	sM->pushState(new OptionsState(app, sM->currentState()->getMngr()->getWorld(), snd));
	sM->currentState()->init();
}

void MenuState::salirMenu(App* game, SoundManager* snd){
	game->exitGame();
}