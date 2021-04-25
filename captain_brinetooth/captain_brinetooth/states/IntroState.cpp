#include "IntroState.h"

IntroState::IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd){

}

IntroState::~IntroState(){
}

void IntroState::init(){
	auto* video = manager_->addEntity();
	VideoPlayer* compVideo = video->addComponent<VideoPlayer>("assets/videos/0001-0120.mp4", true);

	SDL_Rect videoRect = compVideo->getRect();

	auto* fade = manager_->addEntity();
	fade->addComponent<Fade>(Vector2D(videoRect.w, videoRect.h), Vector2D(videoRect.x, videoRect.y), 3000, 3000);
}
