#include "IntroState.h"
#include "MenuState.h"
#include "../sdlutils/SDLUtils.h"

IntroState::IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){

}

IntroState::~IntroState(){
	
}

void IntroState::init() {
	pos = Vector2D(sdlutils().width() / 2, sdlutils().height() / 1.1);
	skip = &sdlutils().msgs().at("anyKey");

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
	std::pair<const char*, std::pair<bool, int>> video1 = { sdlutils().videos().at("logo").c_str(), {false, 20} };
	videos.push_back(video1);
	std::pair<const char*, std::pair<bool, int>> video2 = { sdlutils().videos().at("lore").c_str(), {false, 34} };
	videos.push_back(video2);
	std::pair<const char*, std::pair<bool, int>> video__ = { sdlutils().videos().at("intro").c_str(), {false, 1} };
	videos.push_back(video__);
	std::pair<const char*, std::pair<bool, int>> video_ = { sdlutils().videos().at("introLoop").c_str(), {true, 1} };
	videos.push_back(video_);
	auto* video = manager_->addEntity();
	videoP = video->addComponent<VideoPlayer>(videos);
	//Sonido
	manager_->getSoundMngr()->setGeneralVolume(10,10);
	manager_->getSoundMngr()->playIntroMusic("introStudio");
	manager_->getSoundMngr()->playIntroMusic("cinematica");

}

void IntroState::update(){
	if (!finalSong && videoP->queueSize() == 0)
	{
		manager_->getSoundMngr()->playIntroLoopMusic();
		finalSong = true;
	}
	
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
		sM->changeState(new MenuState(app, sM->currentState()->getMngr()->getWorld(), manager_->getSoundMngr()));
		manager_->getSoundMngr()->stopIntroMusic();

	}

	GameState::update();

	if (fadeComp->getFadeOutComplete()) {
		StateMachine* sM = app->getStateMachine();
		sM->changeState(new MenuState(app, sM->currentState()->getMngr()->getWorld(), manager_->getSoundMngr()));
		manager_->getSoundMngr()->stopIntroMusic();

	}
}

void IntroState::render() const {
	GameState::render();

	float w = skip->width() * 0.3;
	float h = skip->height() * 0.3;
	//Se pone debajo del boton de la tecla E
	Vector2D newPos;
	newPos.setY(pos.getY() - (h / 2));
	newPos.setX(pos.getX() - (w / 2));
	SDL_Rect dest = build_sdlrect(newPos, w, h);
	skip->render(dest);
}


