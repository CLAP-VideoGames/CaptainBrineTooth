#include "components/SoundManager.h"
#include "sdlutils/SDLUtils.h"

void SoundManager::modifyVolume(int volume)
{
	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();

	//if (volume != 0)
	//{
		volumen += volume;
		sdlutils().musics().at(mainMusic).setMusicVolume(volumen);


		while (it != sdlutils().soundEffects().end())
		{
			it->second.setChannelVolume(volumen);
		}
	//}
	/*else
	{
		sdlutils().musics().at(mainMusic).muteMusic();
		sdlutils().musics().at(pauseMusic).muteMusic();

		while (it != sdlutils().soundEffects().end())
		{
			it->second.setChannelVolume(0);
		}
	}*/
}

void SoundManager::ChangeMainMusic(std::string newMusic)
{
	sdlutils().musics().at(mainMusic).haltMusic(); 

	mainMusic = newMusic;
	playMainMusic();
}

void SoundManager::playPauseMusic(std::string mus)
{
	sdlutils().musics().at(mainMusic).pauseMusic();
	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
	while (it != sdlutils().soundEffects().end())
	{
		it->second.pauseChannel();
	}

	sdlutils().musics().at(pauseMusic).play();
}

void SoundManager::resumeMainMusic()
{
	sdlutils().musics().at(pauseMusic).haltMusic();
	sdlutils().musics().at(mainMusic).resumeMusic();
	std::map<std::string, SoundEffect>::iterator it = sdlutils().soundEffects().begin();
	while (it != sdlutils().soundEffects().end())
	{
		it->second.resumeChannel();
	}

}

void SoundManager::playSoundEffect(std::string effect)
{
	sdlutils().soundEffects().at(effect).play();
}