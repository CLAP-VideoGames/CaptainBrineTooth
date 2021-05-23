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
	//points = player->getComponent<Inventory>()->getCoins();
	points = 1600;

	plH = player->getComponent<Player_Health>();

	setExtraLives();
	setExtraHeal();
	setSpines();
	setDoubleDamage();
	setSpeedAttack();
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
	manager_->getSoundMngr()->playSoundEffect("Kit_anzuelos",0);
}

void SkillTreeState::setExtraHeal(){
	skillTree_->setSkill(ExtraHeal, true, points);

	plH->setMaxHeals(plH->getMaxHeals() + 1);
	plH->createHeal();
	manager_->getSoundMngr()->playSoundEffect("Agua_potable", 0);

}

void SkillTreeState::setSpines() {
	skillTree_->setSkill(Spines, true, points);
	skillTree_->setCounterAttackPercentage(0.1f);
	manager_->getSoundMngr()->playSoundEffect("Escamas_Arapaima", 0);

}

void SkillTreeState::setDoubleDamage(){
	skillTree_->setSkill(DoubleDamage, true, points);
	skillTree_->setAttackModifier(2);
	manager_->getSoundMngr()->playSoundEffect("Collar_piranas", 0);
}

void SkillTreeState::setSpeedAttack(){
	skillTree_->setSkill(SpeedAttack, true, points);
	skillTree_->setSpeedModifier(2);
	manager_->getSoundMngr()->playSoundEffect("BrineStone", 0);

}
