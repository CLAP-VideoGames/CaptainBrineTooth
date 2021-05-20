#include "GameState.h"
#pragma once

class SkillTreeState : public GameState
{
public:
	SkillTreeState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);

	virtual void init();
	virtual void render() const;
	virtual void update();

private:
	SDL_Rect cam;
	GameState* stRend;
};