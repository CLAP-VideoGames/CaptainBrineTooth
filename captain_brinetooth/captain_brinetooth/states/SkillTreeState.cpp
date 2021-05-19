#include "SkillTreeState.h"

SkillTreeState::SkillTreeState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) : GameState(a, mundo, snd)
{
	stRend = stateToRender;
	cam = a->camera;
}

void SkillTreeState::init()
{
}

void SkillTreeState::render() const
{
	stRend->render();
	GameState::render();
}

void SkillTreeState::update()
{
	if (ih().keyDownEvent()) {
		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE)) {
			app->getStateMachine()->popState();
		}
	}
	GameState::update();
}
