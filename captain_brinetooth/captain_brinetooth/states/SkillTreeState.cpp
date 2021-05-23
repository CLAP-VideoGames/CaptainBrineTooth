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

	//int w = (int)sdlutils().width() * App::camera_Zoom_Out * 0.35;
	//int h = (int)w * 0.6;	//mantener aspect ratio
	////Creditos
	//auto* creditos = manager_->addEntity();
	//int x_ = (int)((App::camera.w - w) * 0.02);
	//int y_ = (int)((App::camera.h - h) * 0.98);
	//creditos->addComponent<Transform>(Vector2D(x_, y_), Vector2D(0, 0), w * 0.7, h * 0.7, 0.0f);

	//creditos->addComponent<Button>(&sdlutils().images().at("creditos_menu"), activateExtraLives, app, manager_->getSoundMngr());
	//setExtraLives();
	//setExtraHeal();
	//setSpines();
	//setDoubleDamage();
	//setSpeedAttack();
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

void SkillTreeState::activateExtraLives(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setExtraLives();
}

void SkillTreeState::activateExtraHeals(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setExtraHeal();
}

void SkillTreeState::activateSpines(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setSpines();
}

void SkillTreeState::activateDoubleDamage(App* app, SoundManager* snd) {
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setDoubleDamage();
}

void SkillTreeState::activateSpeedAttack(App* app, SoundManager* snd){
	StateMachine* sM = app->getStateMachine();
	SkillTreeState* sTS = dynamic_cast<SkillTreeState*>(sM->currentState());

	if (sTS) sTS->setSpeedAttack();
}

void SkillTreeState::setExtraLives() {
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(ExtraLives, true, points))
		points = player->getComponent<Inventory>()->getCoins();

	
	plH->setMaxLifes(plH->getMaxLifes() + 2);
	plH->resetLifes();
	manager_->getSoundMngr()->playSoundEffect("Kit_anzuelos",0);
}

void SkillTreeState::setExtraHeal(){
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(ExtraHeal, true, points))
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	plH->setMaxHeals(plH->getMaxHeals() + 1);
	plH->createHeal();
	manager_->getSoundMngr()->playSoundEffect("Agua_potable", 0);
}

void SkillTreeState::setSpines() {
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(Spines, true, points))
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	skillTree_->setCounterAttackPercentage(0.1f);
	manager_->getSoundMngr()->playSoundEffect("Escamas_Arapaima", 0);

}

void SkillTreeState::setDoubleDamage(){
	//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(DoubleDamage, true, points)) 
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	skillTree_->setAttackModifier(2);
	manager_->getSoundMngr()->playSoundEffect("Collar_piranas", 0);
}

void SkillTreeState::setSpeedAttack(){//Si se ha podido comprar la habilidad, que se actualicen las monedas
	if(skillTree_->setSkill(SpeedAttack, true, points))
		points = player->getComponent<Inventory>()->getCoins();
	//Accion
	skillTree_->setSpeedModifier(2);
	manager_->getSoundMngr()->playSoundEffect("BrineStone", 0);

}
