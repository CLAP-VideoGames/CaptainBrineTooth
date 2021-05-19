#pragma once

#include "../sdlutils/SDLUtils.h"
#include "../game/App.h"

const int MAX_VOL = 127;
const int MIN_VOL = 0;

class SoundManager{
public:

	SoundManager(int volume, std::string initMus){
		volumenGeneral = volume;
		volumenEfectos = volume;
		volumenPausa = 0;
		timer = 0;
		mainMusic = initMus;
		pauseMusic = initMus + "Pausa";
		cancionPausada = "";
	}

	virtual ~SoundManager() {

	}

	//int MusicVolume() { return volumenMusica; }
	//int EffectsVolume() { return volumenEfectos; }
	std::string currentSong() { return mainMusic; }
	int GeneralVolume() { return volumenGeneral; }
	int PauseVolume() { return volumenPausa; }
	int EffectsVolume() { return volumenEfectos; }

	void setGeneralVolume(int volumeMusic, int volumeEffects); // Poner el volumen GENERAL a cierto nivel especifico
	void setMusicVolume(int volume); // Poner el volumen de la MUSICA a cierto nivel especifico
	void setEffectsVolume(int volume); // Poner el volumen de los EFECTOS a cierto nivel especifico

	void playMainMusic(); // Tocar la melodia que tiene seleccionado el sound manager (Sirve como bucle infinito) 

	void playPauseMusic(); // Detener todo los sonidos y musica del juego principal y tocar la musica del menu de Pausa

	void playIntroMusic() {
		sdlutils().musics().at("fondo_video").setChannelVolume(volumenGeneral);
		sdlutils().musics().at("fondo_video").playforMusic();
	}

	void stopIntroMusic()
	{
		sdlutils().musics().at("fondo_video").haltChannel();
		sdlutils().soundEffects().at("ola").play();
		sdlutils().soundEffects().at("gaviota").play();
	}

	void resumeMainMusic(); // Retomar toda la musica del juego principal despues de detener la musica de Pausa

	void pauseMainMusic(std::string newMusic);

	void ChangeMainMusic(std::string newMusic); // Cambiar de cancion principal

	void playSoundEffect(std::string effect, Uint32 seg);

	void stopMusic();

	inline int getMaxVol() { return MAX_VOL; }

	inline int getMinVol() { return MIN_VOL; }

private:
	std::string mainMusic;
	std::string pauseMusic;

	std::string cancionPausada;

	Uint32 timer;
	int volumenGeneral, volumenPausa, volumenEfectos;
};

