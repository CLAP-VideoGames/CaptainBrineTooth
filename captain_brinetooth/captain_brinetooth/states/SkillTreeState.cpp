#include "SkillTreeState.h"
#include "../components/SkillTree.h"
#include "../components/Inventory.h"

SkillTreeState::SkillTreeState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* player_) : GameState(a, mundo, snd){
	stRend = stateToRender;
	cam = a->camera;
	player = player_;
	skillTree_ = nullptr;
	points = -1;
	
}

void SkillTreeState::init(){
	skillTree_ = player->getComponent<SkillTree>();
	points = player->getComponent<Inventory>()->getCoins();
	skillTree_->setSkill(ExtraLives, true, points);
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
