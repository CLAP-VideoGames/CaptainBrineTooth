#include "SkillTreeState.h"
#include "../components/SkillTree.h"
#include "../components/Inventory.h"
#include "../components/Player_Health.h"

SkillTreeState::SkillTreeState(GameState* stateToRender, App* a, std::shared_ptr<b2World> mundo, SoundManager* snd, Entity* player_) : GameState(a, mundo, snd){
	stRend = stateToRender;
	cam = a->camera;
	player = player_;
	skillTree_ = nullptr;
	points = -1;
	plH = nullptr;
}

void SkillTreeState::init(){
	skillTree_ = player->getComponent<SkillTree>();
	points = player->getComponent<Inventory>()->getCoins();

	plH = player->getComponent<Player_Health>();

	setExtraLives();
	setExtraHeal();
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

void SkillTreeState::setExtraLives() {
	skillTree_->setSkill(ExtraLives, true, points);

	
	plH->setMaxLifes(plH->getMaxLifes() + 2);
	plH->resetLifes();
}

void SkillTreeState::setExtraHeal(){
	skillTree_->setSkill(ExtraHeal, true, points);

	plH->setMaxHeals(plH->getMaxHeals() + 1);
	plH->createHeal();
}
