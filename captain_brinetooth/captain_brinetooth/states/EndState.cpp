#include "EndState.h"
#include "PlayState.h"

EndState::EndState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd): GameState(a, mundo, snd) {
	fadeComp = nullptr;
}

EndState::~EndState()
{
}

void EndState::init() {
	manager_->getSoundMngr()->stopIntroMusic();
	//Para que se sette el Fade
	GameState::init();
	//Fade
	fadeComp = manager_->getHandler<Fader>()->getComponent<Fade>();
	if (fadeComp != nullptr) {
		fadeComp->setTimeIn(1000);
		fadeComp->setTimeOut(1000);
		fadeComp->triggerFade();
	}

	//sdlutils().musics().at("credits_sound").play();
	std::deque<std::pair<const char*, std::pair<bool, int>>> videos;
	//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("end").c_str(), {false, 38} };
	videos.push_back(video__);
	auto* video = manager_->addEntity();
	videoP = video->addComponent<VideoPlayer>(videos);
	videoP->setForcePop(true);
}

void EndState::update() {
	if (ih().mouseButtonEvent()) {
		if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT)) {
			fadeComp->setState(Fade::STATE_FADE::Out);
			fadeComp->triggerFade();
		}
	}

	if (ih().keyDownEvent()) {
		fadeComp->setState(Fade::STATE_FADE::Out);
		fadeComp->triggerFade();

	}

	if (videoP->queueEmpty()) {
		StateMachine* sM = app->getStateMachine();
		manager_->getSoundMngr()->ChangeMainMusic("Nivel1");
		sM->changeState(new PlayState(app, manager_->getWorld(), manager_->getSoundMngr(), true));
	}

	GameState::update();

	if (fadeComp->getFadeOutComplete()) {
		StateMachine* sM = app->getStateMachine();
		manager_->getSoundMngr()->ChangeMainMusic("Nivel1");
		sM->changeState(new PlayState(app, manager_->getWorld(), manager_->getSoundMngr(), true));
	}
}
