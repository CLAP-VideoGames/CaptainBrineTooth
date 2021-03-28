#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/Game.h"
#include "../sdlutils/Music.h"

class SoundManager : public Component {
public:

	SoundManager(int volume, std::string initMus, std::string pausMus)
	{
		volumen = volume;
		mainMusic = initMus;
		pauseMusic = pausMus;
	}
	virtual ~SoundManager() {

	}

	void init() override {}


	void modifyVolume(int volume); // Modificar el volumen

	void playMainMusic() { sdlutils().musics().at(mainMusic).play(); } // Tocar la melodia que tiene seleccionado el sound manager (Sirve como bucle infinito) 
	
	void playPauseMusic(); // Detener todo los sonidos y musica del juego principal y tocar la musica del menu de Pausa
	
	void resumeMainMusic(); // Retomar toda la musica del juego principal despues de detener la musica de Pausa
	
	void ChangeMainMusic(std::string newMusic); // Cambiar de cancion principal
	
	void playSoundEffect(std::string effect);

	// Bucle musica

	// Reproducir efecto sonido

	// Pausar musica

	// Retomar musica

	// sound effects map

private:
	std::string mainMusic;
	std::string pauseMusic;
	int volumen;

};

