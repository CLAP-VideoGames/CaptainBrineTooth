#include "CreditsState.h"
#include "MenuState.h"

CreditsState::CreditsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){ 
	fadeComp = nullptr;
}

CreditsState::~CreditsState() {
}

void CreditsState::init(){
	manager_->getSoundMngr()->stopIntroMusic();
	//Para que se sette el Fade
	GameState::init();
	//Fade
	fadeComp = manager_->getHandler<Fader>()->getComponent<Fade>();
	if (fadeComp != nullptr) {
		fadeComp->setTimeIn(3000);
		fadeComp->setTimeOut(3000);
		fadeComp->triggerFade();
	}

	sdlutils().soundEffects().at("credits_sound").play();

	//Video
	std::deque<std::pair<const char*, std::pair<bool, int>>> videos;
	//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("credits").c_str(), {false, 1} };
	videos.push_back(video__);
	auto* video = manager_->addEntity();
	videoP = video->addComponent<VideoPlayer>(videos);
	videoP->setForcePop(true);
}

void CreditsState::update(){
	if (ih().mouseButtonEvent()) {
		if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)) {
			fadeComp->setState(Fade::STATE_FADE::Out);
			fadeComp->triggerFade();
			
		}
	}

	if (ih().keyDownEvent()){
		fadeComp->setState(Fade::STATE_FADE::Out);
		fadeComp->triggerFade();
		manager_->getSoundMngr()->stopIntroMusic();
	}

	if(videoP->queueEmpty()){ 
		StateMachine* sM = app->getStateMachine();
		sM->changeState(new MenuState(app, sM->currentState()->getMngr()->getWorld(), manager_->getSoundMngr()));
	}

	GameState::update();

	if (fadeComp->getFadeOutComplete()) {
		StateMachine* sM = app->getStateMachine();
		sM->changeState(new MenuState(app, sM->currentState()->getMngr()->getWorld(), manager_->getSoundMngr()));
	}
}
