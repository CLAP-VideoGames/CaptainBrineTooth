#include "IntroState.h"
#include "MenuState.h"

IntroState::IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){

}

IntroState::~IntroState(){
}

void IntroState::init() {
	video = manager_->addEntity();

	std::vector<std::pair<const char*, std::pair<bool, int>>> videos;
	//Filename, loop, frameRate
	std::pair<const char*, std::pair<bool, int>> video__ = { "assets/videos/intro_loop.mp4", {true, 1} };
	videos.push_back(video__);
	std::pair<const char*, std::pair<bool, int>> video_ = { "assets/videos/intro.mp4", {false,1} };
	videos.push_back(video_);

	//VideoPlayer* compVideo = video->addComponent<VideoPlayer>(videos);
	manager_->getSoundMngr()->setGeneralVolume(40);
	manager_->getSoundMngr()->playIntroMusic();
	//SDL_Rect videoRect = compVideo->getRect();
	SDL_Rect videoRect;
	videoRect.x = 0;
	videoRect.y = 0;
	videoRect.w = (int)(App::camera.w * App::camera_Zoom_Out);
	videoRect.h = (int)(App::camera.h * App::camera_Zoom_Out);

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


