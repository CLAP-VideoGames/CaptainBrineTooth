#include "IntroState.h"
#include "MenuState.h"

IntroState::IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){

}

IntroState::~IntroState(){
}

void IntroState::init() {
	//Video
	std::deque<std::pair<const char*, std::pair<bool, int>>> videos;
	//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("intro").c_str(), {false, 1} };
	videos.push_back(video__);
	std::pair<const char*, std::pair<bool, int>> video_ = { sdlutils().videos().at("introLoop").c_str(), {true, 1} };
	videos.push_back(video_);
	auto* video = manager_->addEntity();
	video->addComponent<VideoPlayer>(videos);
	//Sonido
	manager_->getSoundMngr()->setGeneralVolume(10);
	manager_->getSoundMngr()->playIntroMusic();
	//Fade
	fade = manager_->addEntity();
	fadeComp = fade->addComponent<Fade>(Vector2D(App::camera.w, App::camera.h), Vector2D(0, 0), 3000, 3000);
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


