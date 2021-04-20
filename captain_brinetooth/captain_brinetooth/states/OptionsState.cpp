#include "../states/OptionsState.h"
#include "../states/MenuState.h"
#include "../states/PlayState.h"

OptionsState::OptionsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd)
{
	cam = a->camera;

	//No tengo ni idea pero si no es chikito
	cam.w = cam.w * a->getCameraZooOut();
	cam.h = cam.h * a->getCameraZooOut();
}

void OptionsState::init(){
	auto* fondo = manager_->addEntity();

	SDL_Rect posImage;
	posImage.x = 0;
	posImage.y = 0;
	posImage.w = cam.w;
	posImage.h = cam.h;

	fondo->addComponent<Image>(&sdlutils().images().at("fondoOpciones"), posImage, "fondoOpciones");

	manager_->getSoundMngr()->playPauseMusic();

	std::pair<Vector2D, Vector2D> size = {Vector2D(900, 100), Vector2D(200, 200)};
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.2));
	Texture* textures[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume());

	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.5));
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	createEffects(manager_, pos, size, textures1, manager_->getSoundMngr()->EffectsVolume());


	pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX()/2)), cam.h * 0.75));
	Texture* textures2[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()));

	botonVolver();
}

Entity* OptionsState::botonVolver(){
	// Boton de volver al menu
	auto* volver = createBasicEntity(Vector2D(30, cam.h - (cam.h / 5.75)), Vector2D(cam.w - (cam.w / 1.5), cam.h - (cam.h / 1.5)), 0.0f, Vector2D(0, 0));
	volver->addComponent<Button>(&sdlutils().images().at("volverMenu"), volverMenu, app, manager_->getSoundMngr());
	
	return volver;
}

Entity* OptionsState::createVolume(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume){
	Entity* sliderVolume = mngr->addEntity();
	Slider* slider = sliderVolume->addComponent<Slider>(pos, sizes, textures, controlVolume, &sdlutils().images().at("volumen"));

	//Est� mmmuy alto
	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol()/2);
	slider->setSlider(newVol);

	return sliderVolume;
}

Entity* OptionsState::createEffects(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume) {
	Entity* sliderVolume = mngr->addEntity();
	Slider* slider = sliderVolume->addComponent<Slider>(pos, sizes, textures, controlEffects, &sdlutils().images().at("efectos"));

	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol() / 2);
	slider->setSlider(newVol);

	return sliderVolume;
}

Entity* OptionsState::createBrightness(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const float& brightness){
	Entity* sliderBrightness = mngr->addEntity();
	Slider* slider = sliderBrightness->addComponent<Slider>(pos, sizes, textures, controlBrightness, &sdlutils().images().at("brillo"));

	float value = brightness;
	slider->setSlider(value);
	return sliderBrightness;
}

void OptionsState::update() {
	GameState::update(); //esto se podria quitar ya que solo va a hacer un update
}

void OptionsState::controlVolume(float value, Entity* ent){
	SoundManager* snd = ent->getMngr()->getSoundMngr();
	//Est� mmmuy alto
	int newVol = value * (float)(snd->getMaxVol()/2);
	snd->setMusicVolume(newVol);
}

void OptionsState::controlEffects(float value, Entity* ent) {
	SoundManager* snd = ent->getMngr()->getSoundMngr();

	int newVol = value * (float)(snd->getMaxVol() / 2);
	snd->setEffectsVolume(newVol);
}


void OptionsState::controlBrightness(float value, Entity* ent){
	SDL_Window* window = sdlutils().window();
	float newValue = value * (1 - 0.26);
	//Pongo 0.26 porque apartir de ese numero no baja m�s
	if (SDL_SetWindowBrightness(window, newValue + 0.26) != 0){
		std:cout << "Can't lower the brightness";
	}
}

void OptionsState::volverMenu(App* app, SoundManager* snd){
	snd->playSoundEffect("gaviota");
	snd->resumeMainMusic();
	app->getStateMachine()->popState();
}
