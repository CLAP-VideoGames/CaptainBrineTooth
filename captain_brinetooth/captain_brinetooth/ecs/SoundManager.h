#pragma once

#include "../sdlutils/SDLUtils.h"
#include "../game/App.h"
#include "../sdlutils/Music.h"

const int MAX_VOL = 127;
const int MIN_VOL = 0;

class SoundManager{
public:

	SoundManager(int volume, std::string initMus){
		volumenGeneral = volume;
		volumenEfectos = volume;
		volumenPausa = 0;
		mainMusic = initMus;
		pauseMusic = initMus + "Pausa";
	}

	virtual ~SoundManager() {

	}

	//int MusicVolume() { return volumenMusica; }
	//int EffectsVolume() { return volumenEfectos; }
	int GeneralVolume() { return volumenGeneral; }
	int PauseVolume() { return volumenPausa; }
	int EffectsVolume() { return volumenEfectos; }

	void setGeneralVolume(int volume); // Poner el volumen GENERAL a cierto nivel especifico
	void setMusicVolume(int volume); // Poner el volumen de la MUSICA a cierto nivel especifico
	void setEffectsVolume(int volume); // Poner el volumen de los EFECTOS a cierto nivel especifico

	void playMainMusic(); // Tocar la melodia que tiene seleccionado el sound manager (Sirve como bucle infinito) 

	void playPauseMusic(); // Detener todo los sonidos y musica del juego principal y tocar la musica del menu de Pausa

	void resumeMainMusic(); // Retomar toda la musica del juego principal despues de detener la musica de Pausa

	void ChangeMainMusic(std::string newMusic); // Cambiar de cancion principal

	void playSoundEffect(std::string effect) { sdlutils().soundEffects().at(effect).play(); }

	void stopMusic();

	inline int getMaxVol() { return MAX_VOL; }

	inline int getMinVol() { return MIN_VOL; }

private:
	std::string mainMusic;
	std::string pauseMusic;
	int volumenGeneral, volumenPausa, volumenEfectos;
};

