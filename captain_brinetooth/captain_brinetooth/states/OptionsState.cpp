#include "../states/OptionsState.h"
#include "../states/MenuState.h"
#include "../states/PlayState.h"

OptionsState::OptionsState(App* a, b2World* mundo, SoundManager* snd) : GameState(a, mundo, snd)
{
	cam = a->camera;

	//No tengo ni idea pero si no es chikito
	cam.w = cam.w * 2;
	cam.h = cam.h * 2;
}

void OptionsState::init()
{
	auto* newP = manager_->addEntity();
	newP->addComponent<Transform>(Vector2D(0, 0), Vector2D(0, 0), cam.w, cam.h, 0.0f);
	auto* anim_controller = newP->addComponent<AnimBlendGraph>();

	anim_controller->addAnimation("waves", &sdlutils().images().at("fondoMenu"), 11, 11, 1, 1, -1);
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


}

void OptionsState::subirVolumen(App* app, SoundManager* snd)
{
	snd->setGeneralVolume(snd->GeneralVolume() + 5);
}

void OptionsState::bajarVolumen(App* app, SoundManager* snd)
{
	snd->setGeneralVolume(snd->GeneralVolume() - 5);

}

void OptionsState::volverMenu(App* app, SoundManager* snd)
{
	app->getGameStateMachine()->popState();
}
