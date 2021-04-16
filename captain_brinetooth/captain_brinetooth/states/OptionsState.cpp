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

	fondo->addComponent<Image>(&sdlutils().images().at("fondoOpciones"), posImage, "fondoOpciones");

	manager_->getSoundMngr()->ChangeMainMusic("FinalBoss");

	// VOLUMEN
	Entity* BajarV = flechasVolumen();
	Entity* bVolumen = barraVolumen(BajarV->getComponent<Transform>());	
	barcoVolumen(bVolumen->getComponent<Image>()->destino());
	tituloVolumen(BajarV->getComponent<Transform>());

	// BRILLO
	Entity* bajarB = flechasBrillo();
	Entity* bBrillo = barraVolumen(bajarB->getComponent<Transform>());
	barcoBrillo(bBrillo->getComponent<Image>()->destino());
	tituloBrillo(bajarB->getComponent<Transform>());


	Entity* slider = manager_->addEntity();
	Vector2D pos = Vector2D(Vector2D((cam.w * 0.9), cam.h * 0.3));
	std::pair<Vector2D, Vector2D> size = {Vector2D(500, 50), Vector2D(50, 60)};
	Texture* textues[2] = {&sdlutils().images().at("barra") , &sdlutils().images().at("barco")};
	float m = 0.4f;
	slider->addComponent<Slider>(pos, size, textues, controlVolume);
	
	

	botonVolver();
}

Entity* OptionsState::botonVolver()
{
	// Boton de volver al menu
	auto* volver = createBasicEntity(Vector2D(30, cam.h - (cam.h / 5.75)), Vector2D(cam.w - (cam.w / 1.5), cam.h - (cam.h / 1.5)), 0.0f, Vector2D(0, 0));
	volver->addComponent<Button>(&sdlutils().images().at("volverMenu"), volverMenu, app, manager_->getSoundMngr());
	
	return volver;
}

Entity* OptionsState::flechasVolumen()
{
	// Boton de subir el volumen
	auto* subirV = createBasicEntity(Vector2D((cam.w / 1.1), cam.h - cam.h / 1.2), Vector2D(cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2)), 0.0f, Vector2D(0, 0));
	subirV->addComponent<Button>(&sdlutils().images().at("flecha+"), subirVolumen, app, manager_->getSoundMngr());

	// Boton de bajar el volumen
	auto* bajarV = createBasicEntity(Vector2D((cam.w / 3), cam.h - cam.h / 1.2), Vector2D(cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2)), 0.0f, Vector2D(0, 0));
	bajarV->addComponent<Button>(&sdlutils().images().at("flecha-"), bajarVolumen, app, manager_->getSoundMngr());

	return bajarV;
}

Entity* OptionsState::barraVolumen(Transform* e)
{
	// Cogemos la posicion X y modificamos el Y
	Vector2D pos = e->getPos();
	pos.setY(pos.getY() - pos.getY() / 100);

	// Cogemos el ancho y la altura del transform
	float w = e->getW();
	float h = e->getH();

	SDL_Rect posBarra;
	// Modificamos la posicion de la barra para que este ligeramente delante de la flecha -
	posBarra.x = pos.getX() + w / 2;
	posBarra.y = pos.getY() + h / 10;

	// La altura es la misma y la anchura de la barra depende del volumen
	posBarra.h = cam.h - (cam.h / 1.05);
	posBarra.w = manager_->getSoundMngr()->GeneralVolume() * cam.w / 260;

	auto* barraVolumen = manager_->addEntity();
	barraVolumen->addComponent<Image>(&sdlutils().images().at("barra"), posBarra, "bVolumen");

	return barraVolumen;
}

Entity* OptionsState::barcoVolumen(SDL_Rect posBarra)
{
	// Cogemos la posicion de la barra de volumen para colocar bien el Barco
	SDL_Rect posBarco1 = posBarra;
	posBarco1.x += posBarco1.w - cam.w / 12;
	posBarco1.y -= posBarco1.h * 1.5;

	posBarco1.w = cam.w / 10;
	posBarco1.h = cam.h / 10;

	// Barco al final de la barra de volumen
	auto* barco1 = manager_->addEntity();
	barco1->addComponent<Image>(&sdlutils().images().at("barco"), posBarco1, "barco1");

	return barco1;
}

Entity* OptionsState::tituloVolumen(Transform* pos)
{
	// Titulo Volumen
	SDL_Rect posVolumen;
	Vector2D aux = pos->getPos();
	posVolumen.x = aux.getX() - aux.getX() / 1.1;
	posVolumen.y = aux.getY() - aux.getX() / 5;
	posVolumen.h = cam.h - (cam.h / 1.25);
	posVolumen.w = cam.w - cam.w / 1.4;

	auto* volumen = manager_->addEntity();
	volumen->addComponent<Image>(&sdlutils().images().at("volumen"), posVolumen, "volumen");

	return volumen;
}

Entity* OptionsState::flechasBrillo()
{
	auto* subirB = createBasicEntity(Vector2D((cam.w / 1.1), cam.h - cam.h / 2), Vector2D(cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2)), 0.0f, Vector2D(0, 0));
	subirB->addComponent<Button>(&sdlutils().images().at("flecha+"), subirBrillo, app, manager_->getSoundMngr());

	auto* bajarB = createBasicEntity(Vector2D((cam.w / 3), cam.h - cam.h / 2), Vector2D(cam.w - (cam.w / 1.2), cam.h - (cam.h / 1.2)), 0.0f, Vector2D(0, 0));
	bajarB->addComponent<Button>(&sdlutils().images().at("flecha-"), bajarBrillo, app, manager_->getSoundMngr());

	return bajarB;
}

Entity* OptionsState::barraBrillo(Transform* e)
{
	// Cogemos la posicion de la flecha - de brillo
	Vector2D aux;
	aux = e->getPos();
	aux.setY(aux.getY() - aux.getY() / 100);
	float w = e->getW();
	float h = e->getH();

	// Preparamos la posicion y la anchura de la barra de brillo
	SDL_Rect posBrillo;
	posBrillo.x = aux.getX() + w / 2;
	posBrillo.y = aux.getY() + h / 10;

	posBrillo.h = cam.h - (cam.h / 1.05);
	posBrillo.w = SDL_GetWindowBrightness(sdlutils().window()) * cam.w / 8.5;

	auto* brillo = manager_->addEntity();
	brillo->addComponent<Image>(&sdlutils().images().at("barra"), posBrillo, "bBrillo");

	return brillo;
}

Entity* OptionsState::barcoBrillo(SDL_Rect posBrillo)
{
	SDL_Rect posBarco2 = posBrillo;
	posBarco2.x += posBarco2.w - cam.w / 12;
	posBarco2.y -= posBarco2.h * 1.5;

	posBarco2.w = cam.w / 10;
	posBarco2.h = cam.h / 10;

	// Barco al final de la barra de volumen
	auto* barco2 = manager_->addEntity();
	barco2->addComponent<Image>(&sdlutils().images().at("barco"), posBarco2, "barco2");

	return barco2;
}

Entity* OptionsState::tituloBrillo(Transform* pos)
{
	SDL_Rect posTituloBrillo;
	Vector2D posBrillo;
	posBrillo = pos->getPos();
	posTituloBrillo.x = posBrillo.getX() - posBrillo.getX() / 1.1;
	posTituloBrillo.y = posBrillo.getY() - posBrillo.getX() / 5;
	posTituloBrillo.h = cam.h - (cam.h / 1.25);
	posTituloBrillo.w = cam.w - cam.w / 1.4;

	auto* tituloBrillo = manager_->addEntity();
	tituloBrillo->addComponent<Image>(&sdlutils().images().at("brillo"), posTituloBrillo, "brillo");
	
	return tituloBrillo;
}


void OptionsState::update() {

	GameState::update();

	for (Entity* b : manager_->getEnteties()) {
		Button* but = b->getComponent<Button>();
		// Si se hace click al boton y ocurren cosas
		if (but != nullptr && but->handleEvent()){
			// En este punto el OptionsState ya esta destruido
			SDL_Rect aux;
			// Actualizamos todos los elementos de volumen y brillo
			for (Entity* b : manager_->getEnteties()) {
				Image* but = b->getComponent<Image>();
				if (but != nullptr)
				{
					if (but->compareTag("bVolumen"))
					{
						SDL_Rect newPosBarra = but->destino();
						newPosBarra.w = manager_->getSoundMngr()->GeneralVolume() * cam.w / 260;
						aux = newPosBarra;
						but->actualizar(newPosBarra);
					}
					else if (but->compareTag("barco1") || but->compareTag("barco2"))
					{
						SDL_Rect newPosBarco = but->destino();
						newPosBarco.x = aux.x + aux.w - cam.w / 10;
						if (aux.w > 100)
						{
							but->actualizar(newPosBarco);
						}
					}
					else if (but->compareTag("bBrillo"))
					{
						SDL_Rect newPosBarra = but->destino();
						newPosBarra.w = SDL_GetWindowBrightness(sdlutils().window()) * cam.w / 8.5;
						aux = newPosBarra;
						but->actualizar(newPosBarra);
					}
				}
			}
			break;
		}
	}

}

void OptionsState::subirVolumen(App* app, SoundManager* snd)
{
	float vol = snd->GeneralVolume();

	if (vol < 128)
	{
		vol += 12.7;
		snd->setGeneralVolume(vol);
		if (vol >= 128)
		{
			snd->setGeneralVolume(127);
		}
	}
}

void OptionsState::bajarVolumen(App* app, SoundManager* snd)
{
	float vol = snd->GeneralVolume();

	if (vol > 0)
	{
		vol -= 12.7;
		snd->setGeneralVolume(vol);
		if (vol < 0)
		{
			snd->setGeneralVolume(0);
		}
	}
}

void OptionsState::bajarBrillo(App* app, SoundManager* snd)
{
	SDL_Window* window = sdlutils().window();
	if (SDL_SetWindowBrightness(window, SDL_GetWindowBrightness(window) - 0.02f) != 0)
	{
	std:cout << "El brillo baja mas";
	}
}

void OptionsState::subirBrillo(App* app, SoundManager* snd)
{
	SDL_Window* window = sdlutils().window();

	if (SDL_SetWindowBrightness(window, SDL_GetWindowBrightness(window) + 0.02f) != 0)
	{
	std:cout << "El brillo no sube mas";
	}
}

void OptionsState::controlVolume(float value){

	/*std::cout << value << std::endl;
	SoundManager* snd = ent->getMngr().
	float vol = snd->GeneralVolume();

	if (vol < 128)
	{
		vol += 12.7;
		snd->setGeneralVolume(vol);
		if (vol >= 128)
		{
			snd->setGeneralVolume(127);
		}
	}
	else if(vol > 0)
	{
		vol -= 12.7;
		snd->setGeneralVolume(vol);
		if (vol < 0)
		{
			snd->setGeneralVolume(0);
		}
	}*/
}

void OptionsState::volverMenu(App* app, SoundManager* snd)
{
	snd->playSoundEffect("gaviota");
	snd->ChangeMainMusic("Menu");
	app->getStateMachine()->popState();
	
}
