#include "../states/OptionsState.h"
#include "../states/MenuState.h"
#include "../states/PlayState.h"

OptionsState::OptionsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){
	cam = a->camera;
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

	//Boton
	botonVolver();

	//Tamaño de las texturas del Slider
	std::pair<Vector2D, Vector2D> size = { Vector2D(600, 30), Vector2D(95, 100) };
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.2));

	//Volumen
	Texture* textures[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume(), app);

	//Efectos
	pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.55));
	Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	createEffects(manager_, pos, size, textures1, manager_->getSoundMngr()->EffectsVolume(), app);

	//Brillo
	pos = Vector2D(Vector2D((cam.w * 0.5), cam.h * 0.85));
	Texture* textures2[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()), app);
}

Entity* OptionsState::botonVolver(){
	// Boton de volver al menu
	SDL_Rect rectPos;

	Texture* imageTexture = &sdlutils().images().at("volverMenu");
	float sizeFactor = 0.5;
	float factor_ = App::camera_Zoom_Out;
	
	rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(App::camera.w* 0.04 , App::camera.h * 0.8), factor_, sizeFactor, false);
	
	auto* volver = createBasicEntity(Vector2D(rectPos.x, rectPos.y), Vector2D(rectPos.w, rectPos.h), 0.0f, Vector2D(0, 0));
	volver->addComponent<Button>(imageTexture, volverMenu, app, manager_->getSoundMngr());
	
	return volver;
}

Entity* OptionsState::createVolume(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume, App* a){
	Entity* sliderVolume = mngr->addEntity();
	//
	Slider* slider = sliderVolume->addComponent<Slider>(pos, sizes, textures, controlVolume, &sdlutils().images().at("volumen"), a, 0.5f, 50.0f);

	////Est� mmmuy alto
	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol()/2);
	slider->setSlider(newVol);

	return sliderVolume;
}

Entity* OptionsState::createEffects(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume, App* a) {
	Entity* sliderEffects = mngr->addEntity();
	Slider* slider =  sliderEffects->addComponent<Slider>(pos, sizes, textures, controlEffects, &sdlutils().images().at("efectos"), a, 0.5f, 50.0f);

	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol() / 2);
	slider->setSlider(newVol);

	return sliderEffects;
}

Entity* OptionsState::createBrightness(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const float& brightness, App* a){
	Entity* sliderBrightness = mngr->addEntity();
	Slider* slider = sliderBrightness->addComponent<Slider>(pos, sizes, textures, controlBrightness, &sdlutils().images().at("brillo"), a, 0.5f, 30.0f);

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
	snd->playSoundEffect("gaviota",0);
	snd->resumeMainMusic();
	app->getStateMachine()->popState();
}
