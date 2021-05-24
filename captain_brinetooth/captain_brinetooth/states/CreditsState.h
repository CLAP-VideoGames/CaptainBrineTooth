#pragma once
#include "../states/GameState.h"

class CreditsState :public GameState
{
public:
	CreditsState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual ~CreditsState();
	virtual void init();
	virtual void update();

	
	App* getApp() { return app; }
protected:
	Fade* fadeComp;
	VideoPlayer* videoP;
};



