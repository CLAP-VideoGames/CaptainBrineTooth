#pragma once
#include "../states/GameState.h"

class EndState :public GameState
{
public:
	EndState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual ~EndState();
	virtual void init();
	virtual void update();

	App* getApp() { return app; }
protected:
	Fade* fadeComp;
	VideoPlayer* videoP;
};