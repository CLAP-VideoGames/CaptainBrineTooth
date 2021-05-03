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


	//COSAS
	float sizeFactor;
	// Titulo
	auto* title = manager_->addEntity();
	Texture* imageTexture = &sdlutils().images().at("titulo");
	sizeFactor = 0.4;
	float factor_ = app->getCameraZooOut();

	//SDL_Rect rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(cam.w / 1.9, cam.h * 0.07), factor_, sizeFactor, true);
	//title->addComponent<Image>(imageTexture, rectPos, "titulo");
	// 

	//zoom 2		std::pair<Vector2D, Vector2D> size = {Vector2D(500 * App::camera_Zoom_Out, 50*App::camera_Zoom_Out), Vector2D(100 * App::camera_Zoom_Out, 100 * App::camera_Zoom_Out)};
											// Barra			Barco
	std::pair<Vector2D, Vector2D> size = { Vector2D(900, 100), Vector2D(200, 200) };
	

	//zoom 2		Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.25 * App::camera_Zoom_Out));
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.2));

	Texture* textures[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	createVolume(manager_, pos, size, textures, manager_->getSoundMngr()->PauseVolume(), app);

	////zoom 2		pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.55 * App::camera_Zoom_Out));
	//pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.5));

	//Texture* textures1[2] = { &sdlutils().images().at("barra") , &sdlutils().images().at("barco") };
	//createEffects(manager_, pos, size, textures1, manager_->getSoundMngr()->EffectsVolume(), app);


	////zoom 2		pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX()/2)), cam.h * 0.85 * App::camera_Zoom_Out));
	//pos = Vector2D(Vector2D((cam.w * 0.5 - (size.first.getX() / 2)), cam.h * 0.75));

	//Texture* textures2[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	//createBrightness(manager_, pos, size, textures2, SDL_GetWindowBrightness(sdlutils().window()), app);

	//botonVolver();
}

Entity* OptionsState::botonVolver(){
	// Boton de volver al menu
	SDL_Rect rectPos;
	rectPos.x = 0;
	rectPos.y = 0;
	rectPos.w = App::camera.w;
	rectPos.h = App::camera.h;

	Texture* imageTexture = &sdlutils().images().at("volverMenu");
	float sizeFactor = 0.5;
	float factor_ = App::camera_Zoom_Out;
	
	rectPos.x = 0;
	rectPos.y = cam.h;

	rectPos = GameState::ScaleSDL_Rect(imageTexture, Vector2D(App::camera.w / 2, App::camera.h * 0.2), factor_, sizeFactor, false);
	
	auto* volver = createBasicEntity(Vector2D(30, cam.h - (cam.h / 7)), Vector2D(rectPos.w, rectPos.y), 0.0f, Vector2D(0, 0));
	//auto* volver = createBasicEntity(Vector2D(30, cam.h - (cam.h / 5.75)), Vector2D(cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2)), 0.0f, Vector2D(0, 0));
	volver->addComponent<Button>(imageTexture, volverMenu, app, manager_->getSoundMngr());
	
	return volver;
}

Entity* OptionsState::createVolume(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume, App* a){
	Entity* sliderVolume = mngr->addEntity();
	//
	Slider* slider = sliderVolume->addComponent<Slider>(pos, sizes, textures, controlVolume, &sdlutils().images().at("volumen"), a, 0.5f, 1.0f);

	////Est� mmmuy alto
	//float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol()/2);
	////slider->setSlider(newVol);

	return sliderVolume;
}

Entity* OptionsState::createEffects(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const int& volume, App* a) {
	Entity* sliderEffects = mngr->addEntity();
	Slider* slider =  sliderEffects->addComponent<Slider>(pos, sizes, textures, controlVolume, &sdlutils().images().at("efectos"), a, 0.5f, 4.0f);

	float newVol = (float)volume / (float)(mngr->getSoundMngr()->getMaxVol() / 2);
	//slider->setSlider(newVol);

	return sliderEffects;
}

Entity* OptionsState::createBrightness(Manager* mngr, const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes, Texture* textures[2], const float& brightness, App* a){
	Entity* sliderBrightness = mngr->addEntity();
	Slider* slider = sliderBrightness->addComponent<Slider>(pos, sizes, textures, controlVolume, &sdlutils().images().at("brillo"), a, 0.5f, 7.0f);

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
