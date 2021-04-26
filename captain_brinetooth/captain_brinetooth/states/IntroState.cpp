#include "IntroState.h"
#include "MenuState.h"

IntroState::IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){

}

IntroState::~IntroState(){
}

void IntroState::init(){
	//video = manager_->addEntity();
	//VideoPlayer* compVideo = video->addComponent<VideoPlayer>("assets/videos/0001-0120.mp4", true);
	//manager_->getSoundMngr()->setGeneralVolume(40);
	//manager_->getSoundMngr()->playIntroMusic();
	//SDL_Rect videoRect = compVideo->getRect();

	//fade = manager_->addEntity();
	//fadeComp = fade->addComponent<Fade>(Vector2D(videoRect.w, videoRect.h), Vector2D(videoRect.x, videoRect.y), 3000, 3000);
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


