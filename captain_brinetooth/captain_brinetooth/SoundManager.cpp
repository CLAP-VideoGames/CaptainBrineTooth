#include "components/SoundManager.h"
#include "sdlutils/SDLUtils.h"

void SoundManager::setGeneralVolume(int volume)
{

	volumenGeneral = volume;

	setMusicVolume(volumenGeneral);
	setEffectsVolume(volumenGeneral);

}

void SoundManager::setMusicVolume(int volume)
{
	volumenMusica = volume;
	if (volumenMusica >= 0 && volumenMusica < 128)
	{
		sdlutils().musics().at(mainMusic).setMusicVolume(volumenMusica);
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
	sdlutils().musics().at(mainMusic).haltMusic(); 

	mainMusic = newMusic;
	playMainMusic();
}

void SoundManager::playPauseMusic()
{
	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
	while (it != sdlutils().soundEffects().end())
	{
		it->second.pauseChannel();
		++it;
	}



//	sdlutils().musics().at(pauseMusic).playPause(sdlutils().musics().at(mainMusic).getPosition(Ticks o algo por el estilo));
}

void SoundManager::resumeMainMusic()
{
	sdlutils().musics().at(pauseMusic).haltMusic();
	sdlutils().musics().at(mainMusic).resumeMusic();
	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
	while (it != sdlutils().soundEffects().end())
	{
		it->second.resumeChannel();
		++it;

	}

}

