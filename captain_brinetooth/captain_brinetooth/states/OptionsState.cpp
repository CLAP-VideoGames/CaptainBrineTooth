#include "../states/OptionsState.h"
#include "../states/MenuState.h"
#include "../states/PlayState.h"


OptionsState::OptionsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd)
{
	cam = a->camera;

	//No tengo ni idea pero si no es chikito
	cam.w = cam.w * 2;
	cam.h = cam.h * 2;
}

void OptionsState::init()
{
	auto* fondo = manager_->addEntity();

	SDL_Rect posImage;
	posImage.x = 0;
	posImage.y = 0;
	posImage.w = cam.w;
	posImage.h = cam.h;

	fondo->addComponent<Image>(&sdlutils().images().at("fondoOpciones"), posImage);
	
	soundController->ChangeMainMusic("FinalBoss");


	auto* subirV = manager_->addEntity();
	subirV->addComponent<Transform>(Vector2D((cam.w / 1.1) , 300), Vector2D(0, 0), cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2), 0.0f);
	subirV->addComponent<Button>(&sdlutils().images().at("flecha+"), subirVolumen, app, soundController);

	auto* bajarV = manager_->addEntity();
	bajarV->addComponent<Transform>(Vector2D((cam.w / 3), 300), Vector2D(0, 0), cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2), 0.0f);
	bajarV->addComponent<Button>(&sdlutils().images().at("flecha-"), bajarVolumen, app, soundController);

	auto* volver = manager_->addEntity();
	volver->addComponent<Transform>(Vector2D(30, cam.h - (cam.h / 5.75)), Vector2D(0, 0), cam.w - (cam.w / 1.5), cam.h - (cam.h / 1.5), 0.0f);
	volver->addComponent<Button>(&sdlutils().images().at("volverMenu"), volverMenu, app, soundController);

	Vector2D pos = bajarV->getComponent<Transform>()->getPos();
	float w = bajarV->getComponent<Transform>()->getW();
	float h = bajarV->getComponent<Transform>()->getH();

	SDL_Rect posBarra;
	posBarra.x = pos.getX() + w/2;
	posBarra.y = pos.getY() + h/10;

	posBarra.h = cam.h - (cam.h / 1.05);
	posBarra.w = soundController->GeneralVolume() * cam.w / 260;

	auto* barraVolumen = manager_->addEntity();
	barraVolumen->addComponent<Image>(&sdlutils().images().at("barra"), posBarra);

	posBarra.x += posBarra.w - posBarra.w / 1.5;
	posBarra.y -= posBarra.h * 1.5;

	posBarra.w = cam.w / 10;
	posBarra.h = cam.h / 10;

	auto* barco = manager_->addEntity();
	barco->addComponent<Image>(&sdlutils().images().at("barco"), posBarra);
	

}


void OptionsState::update() {

	GameState::update();

	for (Entity* b : manager_->getEnteties()) {
		Button* but = b->getComponent<Button>();
		if (but != nullptr && but->handleEvent())
		{
			break;
		}
	}

}

void OptionsState::subirVolumen(App* app, SoundManager* snd)
{
	snd->setGeneralVolume(snd->GeneralVolume() + 5);
}

void OptionsState::bajarVolumen(App* app, SoundManager* snd)
{
	//snd->setGeneralVolume(snd->GeneralVolume() - 5);
	snd->setGeneralVolume(0);
}

void OptionsState::volverMenu(App* app, SoundManager* snd)
{
	snd->playSoundEffect("gaviota");
	snd->ChangeMainMusic("Menu");
	app->getStateMachine()->popState();
	
}
