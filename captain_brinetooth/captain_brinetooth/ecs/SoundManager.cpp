#include "SoundManager.h"
#include "../sdlutils/SDLUtils.h"

void SoundManager::setGeneralVolume(int volume){
	volumenGeneral = volume;

	setMusicVolume(volumenGeneral);
	setEffectsVolume(volumenGeneral);
}

void SoundManager::setMusicVolume(int volume)
{
	volumenMusica = volume;
	if (volumenMusica >= 0 && volumenMusica < 128)
	{
		sdlutils().musics().at(mainMusic).setChannelVolume(volumenMusica);
	}
	else
	{
		if (volumenMusica < 0) { volumenMusica = 0; }
		else if (volumenMusica >= 128) { volumenMusica = 127; }
	}
}

void SoundManager::setEffectsVolume(int volume)
{
	volumenEfectos = volume;
	if (volumenEfectos >= 0 && volumenEfectos < 128)
	{
		std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
		while (it != sdlutils().soundEffects().end())
		{
			it->second.setChannelVolume(volumenEfectos);
			++it;
		}
	}
	else
	{
		if (volumenEfectos < 0) { volumenEfectos = 0; }
		else if (volumenEfectos >= 128) { volumenEfectos = 127; }
	}


}

void SoundManager::ChangeMainMusic(std::string newMusic)
{
	sdlutils().musics().at(mainMusic).haltChannel();
	sdlutils().musics().at(pauseMusic).haltChannel();

	mainMusic = newMusic;
	pauseMusic = newMusic + "Pausa";

	playMainMusic();
}

void SoundManager::playPauseMusic()
{
	//sdlutils().musics().at(mainMusic).pauseChannel();
	sdlutils().musics().at(mainMusic).setChannelVolume(0);
	//sdlutils().musics().at(mainMusic).resumeChannel();

	
	sdlutils().musics().at(pauseMusic).setChannelVolume(volumenGeneral);
	//sdlutils().musics().at(pauseMusic).pauseChannel();
	//sdlutils().musics().at(pauseMusic).resumeChannel();

}

void SoundManager::resumeMainMusic()
{
	sdlutils().musics().at(pauseMusic).setChannelVolume(0);
	sdlutils().musics().at(mainMusic).setChannelVolume(volumenGeneral);
}

void SoundManager::stopMusic()
{
	sdlutils().musics().at(mainMusic).haltChannel();
}

