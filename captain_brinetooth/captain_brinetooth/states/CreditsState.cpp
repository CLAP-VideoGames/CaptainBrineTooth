#include "CreditsState.h"
#include "MenuState.h"

CreditsState::CreditsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){ 
	fadeComp = nullptr;
}

CreditsState::~CreditsState() {
}

void CreditsState::init(){
	//Para que se sette el Fade
	GameState::init();
	//Fade
	fadeComp = manager_->getHandler<Fader>()->getComponent<Fade>();
	if (fadeComp != nullptr) {
		fadeComp->setTimeIn(3000);
		fadeComp->setTimeOut(3000);
		fadeComp->triggerFade();
	}

	//Video
	std::deque<std::pair<const char*, std::pair<bool, int>>> videos;
	//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("intro").c_str(), {false, 1} };
	videos.push_back(video__);
	std::pair<const char*, std::pair<bool, int>> video_ = { sdlutils().videos().at("introLoop").c_str(), {true, 1} };
	videos.push_back(video_);
	auto* video = manager_->addEntity();
	video->addComponent<VideoPlayer>(videos);
}

void CreditsState::update(){
	if (ih().mouseButtonEvent()) {
		if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)) {
			fadeComp->setState(Fade::STATE_FADE::Out);
			fadeComp->triggerFade();
			manager_->getSoundMngr()->stopIntroMusic();
		}
	}

	GameState::update();

	if (fadeComp->getFadeOutComplete()) {
		StateMachine* sM = app->getStateMachine();
		sM->changeState(new MenuState(app, sM->currentState()->getMngr()->getWorld(), manager_->getSoundMngr()));
	}
}
