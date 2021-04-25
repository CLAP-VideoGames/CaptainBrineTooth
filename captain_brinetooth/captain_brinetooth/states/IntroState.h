#pragma once
#include "../states/GameState.h"

class IntroState : public GameState
{
public:
	IntroState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	~IntroState();

	void init() override;
	void update() override;

private:
	Entity* video = nullptr, *fade = nullptr;
	Fade* fadeComp = nullptr;
};

