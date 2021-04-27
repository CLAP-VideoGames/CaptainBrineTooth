#include "IntroState.h"
#include "MenuState.h"

IntroState::IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){

}

IntroState::~IntroState(){
}

void IntroState::init(){
	video = manager_->addEntity();

	std::vector<std::pair<const char*, bool>> videos;
	std::pair<const char*, bool> video__ = { "assets/videos/IntroTitle_Loop.mp4", true };
	videos.push_back(video__);
	std::pair<const char*, bool> video_ = { "assets/videos/IntroTitle.mp4", false };
	videos.push_back(video_);

	VideoPlayer* compVideo = video->addComponent<VideoPlayer>(videos);
	manager_->getSoundMngr()->setGeneralVolume(40);
	manager_->getSoundMngr()->playIntroMusic();
	SDL_Rect videoRect = compVideo->getRect();

	fade = manager_->addEntity();
	fadeComp = fade->addComponent<Fade>(Vector2D(videoRect.w, videoRect.h), Vector2D(videoRect.x, videoRect.y), 3000, 3000);
}

void IntroState::update(){
	if (ih().mouseButtonEvent()) {
		if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)) {
			fadeComp->setState(Fade::STATE_FADE::Out);
			manager_->getSoundMngr()->stopIntroMusic();
		}
	}

	GameState::update();

	if (fadeComp->getFadeOutComplete()){
		StateMachine* sM = app->getStateMachine();
		sM->changeState(new MenuState(app, sM->currentState()->getMngr()->getWorld(), manager_->getSoundMngr()));
	}
}


